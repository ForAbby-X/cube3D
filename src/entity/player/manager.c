/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:05:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/07 05:20:10 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "entity/player.h"

static inline void	__door_loop(
						t_data *const game,
						t_entity *const self)
{
	t_v3f const	dir = v3frot((t_v3f){1.f}, game->cam.rot);
	t_entity	*ent;
	t_length	len;
	float		t;
	float		w;

	ent = game->entities.data;
	len = game->entities.size;
	while (len > 0)
	{
		if (self != ent && ent->type == ENTITY_DOOR
			&& ray_box_intersection(game->cam.pos, dir, ent->aabb, &t))
		{
			w = cast_ray(&game->map, game->cam.pos, dir, 5).dist;
			t = fminf(w, t);
			if (t < 1.f && w > t && ent->dir[x] < 0.5f)
				ent->dir[x] = 2.f;
		}
		ent++;
		len--;
	}
}

static void	_player_update(
				t_entity *const self,
				t_data *const game,
				float const dt)
{
	player_control(self, game, dt);
	if (ft_mouse(game->eng, 3).pressed)
		__door_loop(game, self);
}

static void	_player_display(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

static void	_player_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

t_entity	*e_player_add(t_data *const game, t_v3f const pos)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_player_update;
	ent->display = &_player_display;
	ent->destroy = &_player_destroy;
	ent->dir = (t_v3f){0};
	ent->rot = (t_v2f){0};
	ent->mesh = &game->models[5];
	ent->aabb = (t_aabb){pos - (t_v3f){0.16f, 0.4125f, 0.16f},
	{0.32f, 0.825f, 0.32f}, AABB_MOVABLE};
	ent->type = ENTITY_PLAYER;
	return (ent);
}
