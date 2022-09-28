/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarjala <ikarjala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 18:20:29 by ikarjala          #+#    #+#             */
/*   Updated: 2022/06/08 15:57:23 by ikarjala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

static inline t_point	offset_size(t_tet *shape, uint16_t size)
{
	return ((t_point){
		(uint8_t)(size - shape->bounds.x),
		(uint8_t)(size - shape->bounds.y)});
}

/*	recursively attempt to fit tetris into map positions
 *	recursing upon successful insert until last tetrimino
 *	backtracking on fail
 *	increase grid size in main if no possible solution
*/
int	solve(uint16_t *map, t_tet *tetris, uint16_t grid_size)
{
	uint64_t	bits;
	t_point		p;
	t_point		size;

	if (!tetris->bits)
		return (FT_TRUE);
	size = offset_size(tetris, grid_size);
	p = (t_point){(uint8_t) - 1, (uint8_t) - 1};
	while (++p.y <= (int8_t)size.y)
	{
		p.x = (uint8_t) - 1;
		while (++p.x <= (int8_t)size.x)
		{
			bits = (tetris->bits << p.x);
			if ((*(t_m4x16 *)&map[p.y] & bits) == 0)
			{
				*(t_m4x16 *)&map[p.y] ^= bits;
				if (solve(map, tetris + 1, grid_size))
					return (pos2d_translate(tetris->atoms, p, 4));
				*(t_m4x16 *)&map[p.y] ^= bits;
			}
		}
	}
	return (FT_FALSE);
}
