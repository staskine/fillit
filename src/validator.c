/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarjala <ikarjala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 13:24:02 by staskine          #+#    #+#             */
/*   Updated: 2022/06/08 15:12:39 by ikarjala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

int	tet_allowed(t_tet shape)
{
	t_uint			index;
	const uint64_t	valid_tetris[19] = {
		I_PIECE, IH_PIECE, O_PIECE,
		L_PIECE, LCW_PIECE, LCCW_PIECE, LUD_PIECE,
		J_PIECE, JCW_PIECE, JCCW_PIECE, JUD_PIECE,
		T_PIECE, TCW_PIECE, TCCW_PIECE, TUD_PIECE,
		S_PIECE, SCW_PIECE, Z_PIECE, ZCW_PIECE
	};

	index = -1U;
	while (++index < 19)
	{
		if (shape.bits == valid_tetris[index])
			return (FT_TRUE);
	}
	return (FT_FALSE);
}

int	check_connections(t_point *atoms, uint8_t n)
{
	t_point	gaps;
	uint8_t	links;
	uint8_t	j;

	links = 0;
	while (n-- > 1)
	{
		j = n;
		while (j-- > 0)
		{
			gaps.x = atoms[n].x - atoms[j].x;
			gaps.y = atoms[n].y - atoms[j].y;
			links += (gaps.x == 1 || gaps.y == 1);
		}
	}
	return (links >= 3);
}

int	check_format(char *buf, t_bool *f_last)
{
	int	i;
	int	atom_c;
	int	nl_c;

	i = 0;
	atom_c = 0;
	nl_c = 0;
	while (buf[i] != '\0')
	{
		if (buf[i] == '#')
			atom_c++;
		else if (buf[i] == '\n')
		{
			if (((i + 1) % 5) == 0 || i == 20)
				nl_c++;
			else
				return (FT_FALSE);
		}
		else if (buf[i] != '.')
			return (FT_FALSE);
		i++;
	}
	*f_last |= nl_c == 4;
	return (atom_c == 4 && (nl_c == 5 || nl_c == 4));
}
