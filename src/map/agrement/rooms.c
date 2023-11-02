/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rooms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:58:47 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/02 19:59:41 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "agrement.h"

static inline void	__fill_zone(
	t_map *const map,
	t_v3i const pos,
	t_vector *const blocks)
{
	if (map_get(map, pos) != cell_air)
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

	if (map_get(map, pos) != cell_air)
		return (0);
	blocks = vector_create(sizeof(t_v3i));
	if (blocks.data == NULL)
		return (0);
	__fill_zone(map, pos, &blocks);
	size = vector_size(&blocks);
	while (vector_size(&blocks))
		map_set(map,
			*(t_v3i *)vector_pop(&blocks), 2048 + size);
	ft_putstr_fd("	New room of ", 1);
	ft_putnbr_fd(size, 1);
	ft_putstr_fd(" blocks found !\n", 1);
	vector_destroy(&blocks);
	return (0);
}

static inline void	__remove_doors_and_refill(
		t_map *const map,
		t_v3i const pos,
		t_cell const size_1,
		t_cell const size_2)
{
	map_set(map, pos, 2048);
	if (map_get(map, pos + (t_v3i){1, 0, 0}) == cell_door_x)
		map_set(map, pos + (t_v3i){1, 0, 0}, 2048);
	if (map_get(map, pos + (t_v3i){-1, 0, 0}) == cell_door_x)
		map_set(map, pos + (t_v3i){-1, 0, 0}, 2048);
	if (map_get(map, pos + (t_v3i){0, 0, 1}) == cell_door_x)
		map_set(map, pos + (t_v3i){0, 0, 1}, 2048);
	if (map_get(map, pos + (t_v3i){0, 0, -1}) == cell_door_x)
		map_set(map, pos + (t_v3i){0, 0, -1}, 2048);
	if (map_get(map, pos + (t_v3i){1, 0, 0}) == cell_door_z)
		map_set(map, pos + (t_v3i){1, 0, 0}, 2048);
	if (map_get(map, pos + (t_v3i){-1, 0, 0}) == cell_door_z)
		map_set(map, pos + (t_v3i){-1, 0, 0}, 2048);
	if (map_get(map, pos + (t_v3i){0, 0, 1}) == cell_door_z)
		map_set(map, pos + (t_v3i){0, 0, 1}, 2048);
	if (map_get(map, pos + (t_v3i){0, 0, -1}) == cell_door_z)
		map_set(map, pos + (t_v3i){0, 0, -1}, 2048);
	ft_putstr_fd("	Connecting a room of ", 1);
	ft_putnbr_fd(size_1, 1);
	ft_putstr_fd(" blocks to a room of ", 1);
	ft_putnbr_fd(size_2, 1);
	ft_putstr_fd(" blocks !\n", 1);
}

int	map_gen_merge_rooms(t_map *const map, t_v3i const pos)
{
	t_cell	size_1;
	t_cell	size_2;

	if (map_get(map, pos) != cell_door_x && map_get(map, pos) != cell_door_z)
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
		__remove_doors_and_refill(map, pos, size_1, size_2);
	return (0);
}
