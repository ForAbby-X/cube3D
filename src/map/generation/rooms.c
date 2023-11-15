/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rooms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 13:55:20 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 09:18:18 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "generation.h"

static inline int	__get_and_remove_zone(
	t_map *const map,
	t_v3i const pos,
	t_vector *const blocks)
{
	int		is_spawn;

	if (map_get(map, pos) < cell_zone)
		return (0);
	is_spawn = (pos[x] == (int)map->spawn[x] && pos[z] == (int)map->spawn[z]);
	map_set(map, pos, cell_air);
	vector_addback(blocks, &(t_v3i){pos[x], pos[y] + 2, pos[z]});
	is_spawn |= __get_and_remove_zone(map, pos + (t_v3i){1, 0, 0}, blocks);
	is_spawn |= __get_and_remove_zone(map, pos + (t_v3i){0, 0, 1}, blocks);
	is_spawn |= __get_and_remove_zone(map, pos + (t_v3i){-1, 0, 0}, blocks);
	is_spawn |= __get_and_remove_zone(map, pos + (t_v3i){0, 0, -1}, blocks);
	return (is_spawn);
}

static inline void	__fill_small_room(t_data *const game, t_vector *const room)
{
	t_v3f		pos;
	float		proba;

	pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size))
		+ (t_v3f){.5f, .0f, .5f};
	proba = ft_rand(0.f, 1.f);
	if (proba < 0.3f)
		e_mimic_add(game, pos, (t_v2f){ft_rand(-M_PI, M_PI)});
	else
	{
		e_enemy_fish_add(game,
			pos + (t_v3f){.25f}, (t_v2f){ft_rand(-M_PI, M_PI)});
		e_enemy_fish_add(game,
			pos + (t_v3f){.0f, .0f, .25f}, (t_v2f){ft_rand(-M_PI, M_PI)});
		e_enemy_fish_add(game,
			pos + (t_v3f){.25f, .0f, .25f}, (t_v2f){ft_rand(-M_PI, M_PI)});
	}
}

static inline void	__fill_medium_room(t_data *const game, t_vector *const room)
{
	int const	max = room->size * 0.1f;
	int			num;
	t_v3f		pos;
	float		proba;

	num = 0;
	while (num < max)
	{
		pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size))
			+ (t_v3f){.5f, .0f, .5f};
		proba = ft_rand(0.f, 1.f);
		if (proba < 0.1f)
			e_enemy_creep_add(game, pos, (t_v2f){ft_rand(-M_PI, M_PI)});
		else if (proba < 0.3f)
			e_enemy_nest_add(game, pos);
		else
			e_enemy_fish_add(game, pos, (t_v2f){ft_rand(-M_PI, M_PI)});
		++num;
	}
}

static inline void	__fill_big_room(t_data *const game, t_vector *const room)
{
	int const	max = room->size * 0.1f;
	int			num;
	t_v3f		pos;
	float		proba;

	num = 0;
	while (num < max)
	{
		pos = v3itof(*(t_v3i *)vector_erase(room, rand() % room->size))
			+ (t_v3f){.5f, .0f, .5f};
		proba = ft_rand(0.f, 1.f);
		if (proba < 0.1f)
			e_enemy_scp_add(game, pos, (t_v2f){ft_rand(-M_PI, M_PI)});
		else if (proba < 0.2f)
			e_enemy_creep_add(game, pos, (t_v2f){ft_rand(-M_PI, M_PI)});
		else if (proba < 0.4f)
			e_enemy_nest_add(game, pos);
		else if (proba < 0.5f)
			e_mimic_add(game, pos, (t_v2f){ft_rand(-M_PI, M_PI)});
		else
			e_enemy_fish_add(game, pos, (t_v2f){ft_rand(-M_PI, M_PI)});
		++num;
	}
}

void	generate_room(t_data *const game, t_v3i const pos)
{
	t_vector		room;

	room = vector_create(sizeof(t_v3i));
	if (room.data == NULL)
		return ;
	if (__get_and_remove_zone(&game->map, pos, &room))
	{
		vector_destroy(&room);
		return ;
	}
	if (room.size < 25)
		__fill_small_room(game, &room);
	else if (room.size < 50)
		__fill_medium_room(game, &room);
	else
		__fill_big_room(game, &room);
	vector_destroy(&room);
}
