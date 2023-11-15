/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rope.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:12:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 10:13:11 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "generation.h"

static inline void	__get_and_remove_zone(
	t_map *const map,
	t_v3i const pos,
	t_vector *const blocks)
{
	if (map_get(map, pos) == cell_wall)
		return ;
	map_set(map, pos, cell_wall);
	vector_addback(blocks, &(t_v3i){pos[x], pos[y] + 2, pos[z]});
	__get_and_remove_zone(map, pos + (t_v3i){1, 0, 0}, blocks);
	__get_and_remove_zone(map, pos + (t_v3i){0, 0, 1}, blocks);
	__get_and_remove_zone(map, pos + (t_v3i){-1, 0, 0}, blocks);
	__get_and_remove_zone(map, pos + (t_v3i){0, 0, -1}, blocks);
}

static inline t_v3i	__get_furthest(t_v3i const start, t_vector *const room)
{
	t_v3i	*actual;
	int		max_dist;
	t_v3i	furthest;
	t_v3i	diff;

	max_dist = 0;
	while (room->size)
	{
		actual = vector_pop(room);
		diff = *actual - start;
		if (diff[x] < 0)
			diff[x] = -diff[x];
		if (diff[z] < 0)
			diff[z] = -diff[z];
		if (diff[x] + diff[z] > max_dist)
		{
			max_dist = diff[x] + diff[z];
			furthest = *actual;
		}
	}
	return (furthest);
}

int	generate_rope(t_data *const game)
{
	t_v3i		start;
	t_v3i		furthest;
	t_vector	room;

	room = vector_create(sizeof(t_v3i));
	if (room.data == NULL)
		return (1);
	start = v3ftoi(game->map.spawn) - (t_v3i){0, 2, 0};
	__get_and_remove_zone(&game->map, start, &room);
	furthest = __get_furthest(start, &room);
	e_rope_add(game, v3itof(furthest) + (t_v3f){.5f, 0.f, .5f});
	vector_destroy(&room);
	return (0);
}
