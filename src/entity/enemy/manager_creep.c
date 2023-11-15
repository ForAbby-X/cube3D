/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_creep.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 03:09:47 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/14 14:12:39 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "particle/particle.h"

static inline void	__attack(
						t_entity *const self,
						t_data *const game,
						float const dt)
{
	t_entity *const	player = game->entities.data;

	self->dir[y] += dt;
	if (self->dir[y] >= 3.f)
	{
		self->dir[y] = 0.f;
		if (can_see_aabb(game, self->aabb.pos + self->aabb.dim / 2.f,
				&player->aabb, 999.f))
		{
			particle_line(game, self->aabb.pos + self->aabb.dim / 2.f,
				player->aabb.pos + player->aabb.dim / 2.f, game->sprites[6]);
			p_small_exp(game, player->aabb.pos + player->aabb.dim / 2.f, 40,
				game->sprites[6]);
		player->health -= ft_rand(30.f, 40.f);
		}
	}
}

static void	_enemy_creep_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	t_entity *const	player = game->entities.data;
	const t_v3f		self_center = self->aabb.pos + self->aabb.dim / 2.f;
	const t_v3f		player_center = player->aabb.pos + player->aabb.dim / 2.f;
	const t_v3f		diff = player_center - self_center;
	const float		dist = v3fmag(diff);

	self->rot[x] = atan2f(diff[z], diff[x]);
	if (self->dir[x] >= 0.3f)
	{
		p_zone_add(game, self->aabb, 3, game->sprites[6]);
		self->dir[x] = 0.f;
	}
	self->dir[x] += dt;
	if (dist >= 2.5f)
	{
		self->rot[y] = sinf(self->time_alive) * 0.15f;
		self->rot[x] += sinf(self->time_alive * 1.4f) * 0.15f;
		self->vel = v3fnorm((t_v3f){diff[x], 0.f, diff[z]}, dt * .6f);
	}
	else
	{
		self->rot[y] = asinf(diff[y] / dist);
		__attack(self, game, dt);
	}
}

static void	_enemy_creep_display(t_entity *const self, t_data *const game)
{
	t_transform	transform;

	transform.resize = (t_v3f){.3f, .3f, .3f};
	transform.rotation = self->rot;
	transform.translation = self->aabb.pos + self->aabb.dim / 2.f;
	mesh_put(game->eng, &game->cam, transform, &game->models[14]);
}

t_entity	*e_enemy_creep_add(t_data *const game, t_v3f const pos, t_v2f rot)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_enemy_creep_update;
	ent->display = &_enemy_creep_display;
	ent->dir = (t_v3f){0.f};
	ent->rot = rot;
	ent->max_health = 150.f;
	ent->health = ent->max_health;
	ent->aabb = (t_aabb){pos - (t_v3f){.3f, .4f, .3f}, \
		{.6f, .6f, .6f}, AABB_MOVABLE};
	if (map_get(&game->map, v3ftoi(pos) + (t_v3i){0, 1, 0}) == cell_air)
	ent->aabb = (t_aabb){pos + (t_v3f){0, 1} - (t_v3f){.3f, .0f, .3f}, \
		{.6f, .6f, .6f}, AABB_MOVABLE};
	ent->mesh = &game->models[0];
	ent->type = ENTITY_ENNEMY_CREEP;
	return (ent);
}
