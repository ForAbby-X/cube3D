/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:05:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 17:49:31 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "entity/player.h"
#include "particle/particle.h"
#include "gameplay_utils.h"

static void	_player_update(
				t_entity *const self,
				t_data *const game,
				float const dt)
{
	t_entity *const	player = game->entities.data;

	player_control(self, game, dt);
	game->cam.pos = self->aabb.pos + (t_v3f){0.16f, 0.7f, 0.16f};
	game->cam.pos[y] += sinf(game->holding.energy_vel * 5.f) * 0.03f;
	game->cam.rot = self->rot;
	if (self->health <= 0.0f && game->state <= 1)
	{
		player->aabb.type = AABB_IMMOVABLE;
		player->aabb.dim[y] = 0.15f;
		game->cam.fog_distance = 3.f;
		game->cam.fog_color = (t_color){0x0};
		game->death_pos = self->aabb.pos + self->aabb.dim * .5f
			+ (t_v3f){0.f, .5f};
		game->state = 2;
		title_put(&game->title, g_titles[3], 10.0f);
	}
	self->aabb.pos[y] = 3.f;
}

static void	_player_display(t_entity *const self, t_data *const game)
{
	t_transform		trans;

	trans.rotation = (t_v2f){0.f, 0.f};
	trans.resize = (t_v3f){.6f, .6f, .6f};
	if (game->state == 2 && self->health <= 0.0f)
	{
		trans.translation = game->death_pos;
		trans.translation[y] = 3.f;
		mesh_put(game->eng, &game->cam, trans, &game->models[17]);
	}
}

static void	_player_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

t_entity	*e_player_add(t_data *const game, t_v3f const pos, t_v2f const rot)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_player_update;
	ent->display = &_player_display;
	ent->destroy = &_player_destroy;
	ent->dir = (t_v3f){0};
	ent->rot = rot;
	ent->max_health = 300.f;
	ent->health = ent->max_health;
	ent->mesh = &game->models[5];
	ent->aabb = (t_aabb){pos - (t_v3f){0.16f, 0.f, 0.16f},
	{0.32f, 0.825f, 0.32f}, AABB_MOVABLE};
	ent->type = ENTITY_PLAYER;
	return (ent);
}
