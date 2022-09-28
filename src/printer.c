/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarjala <ikarjala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 18:19:51 by ikarjala          #+#    #+#             */
/*   Updated: 2022/06/08 15:28:11 by ikarjala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

int	display_error(void)
{
	ft_putendl_fd("error", FD_OUT);
	return (XC_ERROR);
}

int	display_usage(void)
{
	const char	*msg = "usage: fillit <tetrimino_file>";

	ft_putendl_fd(msg, FD_OUT);
	return (XC_EXIT);
}

static inline void	put_tetris(char *buf, t_tet *tetris, uint16_t grid_size)
{
	char		legend;
	t_uint		atom_c;
	t_point		pos;

	legend = 'A';
	while (tetris->bits)
	{
		atom_c = 4;
		while (atom_c-- > 0)
		{
			pos = tetris->atoms[atom_c];
			buf[(uint16_t)(pos.y * grid_size + pos.x + pos.y)] = legend;
		}
		legend++;
		tetris = &tetris[1];
	}
}

int	print_grid(uint16_t size, t_tet *tetris)
{
	char		*buf;
	uint16_t	sqr;
	uint16_t	i;

	sqr = size * size + size;
	buf = (char *)malloc(sizeof(char) * sqr);
	if (!buf)
		return (XC_ERROR);
	ft_memset(buf, '.', sqr);
	i = size;
	while (i <= sqr)
	{
		buf[i] = '\n';
		i += size + 1;
	}
	put_tetris(buf, tetris, size);
	write(FD_OUT, buf, sqr);
	ft_memdel((void **)&buf);
	return (XC_EXIT);
}
