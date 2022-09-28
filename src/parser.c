/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarjala <ikarjala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 15:52:54 by ikarjala          #+#    #+#             */
/*   Updated: 2022/06/08 15:52:55 by ikarjala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*	convert atom positions to m4x16 bitstring code
 *	in index -> RTL order
 *	(top-left corner being represented by the rightmost bit)
*/
static	uint64_t	to_bitstr64(t_point *atoms, uint8_t n)
{
	uint64_t	bits;
	uint8_t		toggle_index;

	bits = 0x0;
	while (n-- > 0)
	{
		toggle_index = atoms[n].y * 16 + atoms[n].x;
		bits |= (uint64_t)(1ULL << toggle_index);
	}
	return (bits);
}

static t_point	get_bounds(t_point *atoms, uint8_t n)
{
	t_point		bounds;
	t_point		max;
	uint8_t		xmin;

	xmin = 5;
	max.x = 0;
	max.y = atoms[n - 1].y;
	while (n-- > 0)
	{
		max.x = (uint8_t)ft_max(max.x, atoms[n].x);
		xmin = (uint8_t)ft_min(xmin, atoms[n].x);
	}
	bounds.x = (max.x - xmin + 1);
	bounds.y = (max.y - atoms[0].y + 1);
	return (bounds);
}

static t_point	*get_atoms_coords(char *buf, t_point *atoms)
{
	t_uint	a_i;
	t_uint	buf_i;
	uint8_t	xoff;

	xoff = 5;
	a_i = -1U;
	buf_i = -1U;
	while (buf[++buf_i])
	{
		if (buf[buf_i] == '#')
		{
			atoms[++a_i] = (t_point){
				(uint8_t)(buf_i % 5), (uint8_t)(buf_i / 5)};
			xoff = (uint8_t)ft_min(xoff, atoms[a_i].x);
		}
	}
	pos2d_translate(atoms, (t_point){-xoff, -atoms->y}, 4);
	return (atoms);
}

static inline int	read_equ(int fd, void *buf, ssize_t *o_len)
{
	*o_len = read(fd, buf, BUFF_SIZE);
	if (*o_len > 0)
		((char *)buf)[*o_len] = 0;
	return (*o_len != 0);
}

/*	loop through file
 *	check string format (newlines/invalid characters)
 *	set atom coordinates
 *	convert tetrimino buffer to bitstring
 *	calculate tetrimino bounds
 *	check tetrimino validity (against table)
*/
int	parse(int fd, t_tet *tetris, uint8_t *tet_count)
{
	char	buf[BUFF_SIZE + 1];
	t_point	*atoms;
	ssize_t	r_len;
	uint8_t	tet_i;
	t_bool	f_last;

	f_last = FT_FALSE;
	tet_i = (uint8_t)-1;
	while (read_equ(fd, buf, &r_len))
	{
		if ((f_last || ++tet_i >= MAX_TETRIS || r_len < BUFF_SIZE - 1)
			|| (!check_format(buf, &f_last) || !check_connections(
					get_atoms_coords(buf, tetris[tet_i].atoms), 4)))
			return (XC_ERROR);
		atoms = tetris[tet_i].atoms;
		tetris[tet_i].bits = to_bitstr64(atoms, 4);
		tetris[tet_i].bounds = get_bounds(atoms, 4);
		if (!tet_allowed(tetris[tet_i]))
			return (XC_ERROR);
	}
	close(fd);
	*tet_count = tet_i + 1;
	if (!f_last)
		return (XC_ERROR);
	return (XC_EXIT);
}
