/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarjala <ikarjala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 18:19:57 by ikarjala          #+#    #+#             */
/*   Updated: 2022/06/08 15:28:42 by ikarjala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLIT_H
# define FILLIT_H

# include <fcntl.h>
# include <stdint.h>
# include "libft.h"

# define FD_IN	0
# define FD_OUT	1
# define FD_ERR	2

# define XC_ERROR	-1
# define XC_EXIT	0

# define MAX_TETRIS	26
# define BUFF_SIZE	21

# define MAP_PADDING 4

/* GLOSSARY
 * bounds:	width and height of a tetrimino
 * atoms :	individual blocks of a tetrimino
 * t_m4x16:	64-bit value representing a 4x16 matrix as individual bits
 *
 * NOTE:	bits are RTL
 * NOTE:	map origin is TopLeft
*/

typedef struct s_tuple
{
	uint8_t	x;
	uint8_t	y;
}	t_point;

typedef uint64_t	t_m4x16;
typedef struct s_tetrimino
{
	uint64_t	bits;
	t_point		bounds;
	t_point		atoms[4];
}	t_tet;

/* TETRIMINO CODES */
# define I_PIECE	0x0001000100010001ULL
# define IH_PIECE	0x000000000000000FULL
# define O_PIECE	0x0000000000030003ULL

# define L_PIECE	0x0000000300010001ULL
# define LCW_PIECE	0x0000000000010007ULL
# define LCCW_PIECE	0x0000000000070004ULL
# define LUD_PIECE	0x0000000200020003ULL

# define J_PIECE	0x0000000300020002ULL
# define JCW_PIECE	0x0000000000070001ULL
# define JCCW_PIECE	0x0000000000040007ULL
# define JUD_PIECE	0x0000000100010003ULL

# define T_PIECE	0x0000000000020007ULL
# define TCW_PIECE	0x0000000200030002ULL
# define TCCW_PIECE	0x0000000100030001ULL
# define TUD_PIECE	0x0000000000070002ULL

# define S_PIECE	0x0000000000030006ULL
# define SCW_PIECE	0x0000000200030001ULL
# define Z_PIECE	0x0000000000060003ULL
# define ZCW_PIECE	0x0000000100030002ULL

/* PRINTER */

int		display_error(void);
int		display_usage(void);
int		print_grid(uint16_t size, t_tet *tetris);

/* PARSER */

int		parse(int fd, t_tet *tetris, u_int8_t *tet_count);
/* VALIDATOR */

int		tet_allowed(t_tet shape);
int		check_connections(t_point *atoms, uint8_t n);
int		check_format(char *buf, t_bool *f_last);

/* SOLVER */

int		solve(uint16_t *map, t_tet *tetris, uint16_t size);

/* MATH */

int		ft_min(int a, int b);
int		ft_max(int a, int b);
int		ft_sqrt(int number);
int		pos2d_translate(t_point *atoms, t_point pos, uint8_t n);

#endif
