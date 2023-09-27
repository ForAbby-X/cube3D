/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agrement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 13:08:16 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/27 13:25:59 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

static inline void	__doors(t_map *const map, t_v3i const pos)
{
	if ((map_get(map, pos + (t_v3i){1, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){-1, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){0, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, -1}) == 0)
		|| (map_get(map, pos + (t_v3i){0, 0, 1}) == 1
		&& map_get(map, pos + (t_v3i){0, 0, -1}) == 1
		&& (map_get(map, pos + (t_v3i){1, 0, 0}) == 0
		&& map_get(map, pos + (t_v3i){-1, 0, 0}) == 0)))
		map_set(map, pos, cell_door);
}

void	map_agrement(t_map *const map)
{
	t_v3i	pos;

	pos[y] = 1;
	pos[z] = 0;
	while (pos[z] < map->size[z])
	{
		pos[x] = 0;
		while (pos[x] < map->size[x])
		{
			if (map_get(map, pos) == 0)
				__doors(map, pos);
			pos[x]++;
		}
		pos[z]++;
	}
}
