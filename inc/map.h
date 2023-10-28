/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:37:59 by vmuller           #+#    #+#             */
/*   Updated: 2023/10/28 15:15:35 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include "engine.h"
# include "vec3.h"

# define MAX_SPRITES 6

enum	e_cell
{
	cell_air = 0,
	cell_wall = 1,
	cell_door_x = 253,
	cell_door_z = 254,
	cell_void = 255,
	cell_zone = 2048
};

typedef unsigned int	t_cell;
typedef struct s_map	t_map;

t_map	map_create(t_v3i const size);
void	map_destroy(t_map *const map);

t_cell	map_get(t_map const *const map, t_v3i const pos);
t_v3i	map_size(t_map const *const map);

void	map_set(t_map *const map, t_v3i const pos, t_cell const cell);
void	map_fill(t_map *const map, t_v3i pos1, t_v3i pos2, t_cell const cell);

int		map_iterate(
			t_map *const map,
			int (*func)(t_map *const map, t_v3i const pos));

void	map_generate(t_data *const game);

struct s_map
{
	t_cell		***data;
	t_v3i		size;
	t_v3f		spawn;
	t_sprite	*sprites[6];
	t_color		sky_color;
	t_color		ground_color;
};

#endif
