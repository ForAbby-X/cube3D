/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agrement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 13:08:16 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/28 22:24:56 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

static int	_put_doors(t_map *const map, t_v3i const pos)
{
	if (map_get(map, pos) != 0)
		return (0);
	if (map_get(map, pos + (t_v3i){1, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){-1, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){0, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, -1}) == 0)
		map_set(map, pos, cell_door);
	if (map_get(map, pos + (t_v3i){0, 0, 1}) == 1
		&& map_get(map, pos + (t_v3i){0, 0, -1}) == 1
		&& map_get(map, pos + (t_v3i){1, 0, 0}) == 0
		&& map_get(map, pos + (t_v3i){-1, 0, 0}) == 0)
		map_set(map, pos, cell_door);
	if (map_get(map, pos + (t_v3i){-1, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){1, 0, 0}) == 0
		&& map_get(map, pos + (t_v3i){2, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){1, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){1, 0, -1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, -1}) == 0)
		map_fill(map, pos, pos + (t_v3i){1, 0, 0}, cell_door);
	if (map_get(map, pos + (t_v3i){0, 0, -1}) == 1
		&& map_get(map, pos + (t_v3i){0, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, 2}) == 1
		&& map_get(map, pos + (t_v3i){1, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){-1, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){1, 0, 0}) == 0
		&& map_get(map, pos + (t_v3i){-1, 0, 0}) == 0)
		map_fill(map, pos, pos + (t_v3i){0, 0, 1}, cell_door);
	return (0);
}

static inline void	__fill_zone(
	t_map *const map,
	t_v3i const pos,
	t_cell *const size)
{
	t_cell const	block = map_get(map, pos);

	if (block != cell_wall && block != cell_door && block < *size)
		map_set(map, pos, 2048 + *size);
	if (block != cell_air)
		return ;
	++(*size);
	__fill_zone(map, pos + (t_v3i){1, 0, 0}, size);
	__fill_zone(map, pos + (t_v3i){0, 0, 1}, size);
	__fill_zone(map, pos + (t_v3i){-1, 0, 0}, size);
	__fill_zone(map, pos + (t_v3i){0, 0, -1}, size);
	if (map_get(map, pos) < 2048 + *size)
		map_set(map, pos, 2048 + *size);
}

static int	_put_zones(t_map *const map, t_v3i const pos)
{
	t_cell	size;

	size = 2048;
	if (map_get(map, pos) != cell_air)
		return (0);
	__fill_zone(map, pos, &size);
	printf("Zone size:%u\n", map_get(map, pos));
	return (0);
}

void	map_agrement(t_map *const map)
{
	map_iterate(map, _put_doors);
	map_iterate(map, _put_zones);
}
