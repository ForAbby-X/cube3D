/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rooms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 13:55:20 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/07 05:43:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "generation.h"

static inline void	__get_and_remove_zone(
	t_map *const map,
	t_v3i const pos,
	t_vector *const blocks)
{
	if (map_get(map, pos) < cell_zone)
		return ;
	map_set(map, pos, cell_air);
	vector_addback(blocks, &(t_v3i){pos[x], pos[y] + 2, pos[z]});
	__get_and_remove_zone(map, pos + (t_v3i){1, 0, 0}, blocks);
	__get_and_remove_zone(map, pos + (t_v3i){0, 0, 1}, blocks);
	__get_and_remove_zone(map, pos + (t_v3i){-1, 0, 0}, blocks);
	__get_and_remove_zone(map, pos + (t_v3i){0, 0, -1}, blocks);
}

static inline void	__fill_small_room(t_data *const game, t_vector *const room)
{
	t_v3f	pos;

	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
}

static inline void	__fill_medium_room(t_data *const game, t_vector *const room)
{
	t_v3f	pos;

	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
}

static inline void	__fill_big_room(t_data *const game, t_vector *const room)
{
	t_v3f	pos;

	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size));
	e_mimic_add(game, pos + (t_v3f){.5f, .0f, .5f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
}

void	generate_room(t_data *const game, t_v3i const pos)
{
	t_vector		room;

	room = vector_create(sizeof(t_v3i));
	if (room.data == NULL)
		return ;
	__get_and_remove_zone(&game->map, pos, &room);
	if (room.size < 25)
		__fill_small_room(game, &room);
	else if (room.size < 50)
		__fill_medium_room(game, &room);
	else if (room.size < 100)
		__fill_big_room(game, &room);
	vector_destroy(&room);
}
