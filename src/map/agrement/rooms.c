/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rooms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:58:47 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/29 16:45:27 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "agrement.h"

static inline void	__fill_zone(
	t_map *const map,
	t_v3i const pos,
	t_vector *const blocks)
{
	t_cell const	block = map_get(map, pos);

	if (block != cell_air)
		return ;
	map_set(map, pos, 2048);
	vector_addback(blocks, (void *)&pos);
	__fill_zone(map, pos + (t_v3i){1, 0, 0}, blocks);
	__fill_zone(map, pos + (t_v3i){0, 0, 1}, blocks);
	__fill_zone(map, pos + (t_v3i){-1, 0, 0}, blocks);
	__fill_zone(map, pos + (t_v3i){0, 0, -1}, blocks);
}

int	map_gen_rooms(t_map *const map, t_v3i const pos)
{
	t_length	size;
	t_vector	blocks;

	size = 2048;
	blocks = vector_create(sizeof(t_v3i));
	if (blocks.data == NULL)
		return (0);
	if (map_get(map, pos) != cell_air)
		return (0);
	__fill_zone(map, pos, &blocks);
	size = vector_size(&blocks);
	while (vector_size(&blocks))
		map_set(map, *(t_v3i *)vector_pop(&blocks), 2048 + size);
	printf("Zone size:%u\n", size);
	vector_destroy(&blocks);
	return (0);
}

static inline void	__remove_doors_and_refill(t_map *const map, t_v3i const pos)
{
	map_set(map, pos, cell_air);
	if (map_get(map, pos + (t_v3i){1, 0, 0}) == cell_door)
		map_set(map, pos + (t_v3i){1, 0, 0}, 2049);
	if (map_get(map, pos + (t_v3i){-1, 0, 0}) == cell_door)
		map_set(map, pos + (t_v3i){-1, 0, 0}, 2049);
	if (map_get(map, pos + (t_v3i){0, 0, 1}) == cell_door)
		map_set(map, pos + (t_v3i){0, 0, 1}, 2049);
	if (map_get(map, pos + (t_v3i){0, 0, -1}) == cell_door)
		map_set(map, pos + (t_v3i){0, 0, -1}, 2049);
	printf("Merged two rooms together !\n");
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
	if (size_1 + size_2 < 25)
		__remove_doors_and_refill(map, pos);
	return (0);
}

int	map_gen_elevate_rooms(t_map *const map, t_v3i const pos)
{
	t_cell	block;

	block = map_get(map, pos);
	if (block == cell_door)
	{
		map_set(map, pos, cell_air);
		map_set(map, pos + (t_v3i){0, -1, 0}, cell_door);
	}
	if (block < 2048)
		return (0);
	block -= 2048;
	if (block < 6)
		map_fill(map, pos, pos, cell_air);
	else if (block < 50)
		map_fill(map, pos, pos + (t_v3i){0, 1, 0}, cell_air);
	else if (block < 65)
		map_fill(map, pos, pos + (t_v3i){0, 2, 0}, cell_air);
	else
		map_fill(map, pos, pos + (t_v3i){0, 3, 0}, cell_air);
	return (0);
}
