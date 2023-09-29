/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rooms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:58:47 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/29 12:31:53 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "agrement.h"

static inline void	__fill_zone(
	t_map *const map,
	t_v3i const pos,
	t_cell *const size)
{
	t_cell const	block = map_get(map, pos);

	if (block != cell_air || block - 2048 < *size)
		return ;
	map_set(map, pos, 2048);
	++(*size);
	__fill_zone(map, pos + (t_v3i){1, 0, 0}, size);
	__fill_zone(map, pos + (t_v3i){0, 0, 1}, size);
	__fill_zone(map, pos + (t_v3i){-1, 0, 0}, size);
	__fill_zone(map, pos + (t_v3i){0, 0, -1}, size);
	map_set(map, pos, 2048 + *size);
}

int	map_gen_rooms(t_map *const map, t_v3i const pos)
{
	t_cell	size;

	size = 2048;
	if (map_get(map, pos) != cell_air)
		return (0);
	__fill_zone(map, pos, &size);
	printf("Zone size:%u\n", size - 2048);
	return (0);
}

static inline void	__remove_doors_and_refill(t_map *const map, t_v3i const pos)
{
	
}

int	map_gen_merge_rooms(t_map *const map, t_v3i const pos)
{
	t_cell	size_1;
	t_cell	size_2;

	if (map_get(map, pos) != cell_door)
		return (0);
	if (map_get(map, pos + (t_v3i){1, 0, 0}) >= 2048)
	{
		size_1 = map_get(map, pos + (t_v3i){1, 0, 0}) - 2048;
		size_2 = map_get(map, pos + (t_v3i){-1, 0, 0}) - 2048;
	}
	else
	{
		size_1 = map_get(map, pos + (t_v3i){0, 0, 1}) - 2048;
		size_2 = map_get(map, pos + (t_v3i){0, 0, -1}) - 2048;
	}
	if (size_1 < 15 && size_2 < 15)
		__remove_doors_(map, pos);
	return (0);
}
