/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_fish.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 19:39:50 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/14 18:26:21 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "particle/particle.h"

static inline void	__attack(
			t_data *const game,
			t_entity *const self,
			float const dt,
			float const dist)
{
	t_entity *const	player = game->entities.data;

	if (self->dir[y] >= 1.f && dist < 0.60f)
	{
		player->health -= 3.f;
		self->dir[y] = -0.5f;
	}
	self->dir[y] += dt;
}

static void	_enemy_fish_part(
			t_data *const game,
			t_entity *const	self,
			const t_v3f	diff)
{
	const t_v3f		self_center = self->aabb.pos + self->aabb.dim / 2.f;
	const float		dist = v3fmag(diff);
	t_particle		*part;

	self->dir[x] = 0.f;
	part = particle_add(game, self_center + (t_v3f) \
		{ft_rand(-0.15f, 0.150f), ft_rand(-0.15f, 0.08f), \
		ft_rand(-0.15f, 0.150f)} + diff / dist * 0.2f, \
		(t_v3f){0.f, ft_rand(-.3f, -.15f)});
	if (part == NULL)
		return ;
	part->force = 5.0f;
	part->spr = game->sprites[5];
	part->death_time = ft_rand(.5f, 0.7f);
}

static void	_enemy_fish_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	t_entity *const	player = game->entities.data;
	const t_v3f		self_center = self->aabb.pos + self->aabb.dim / 2.f;
	const t_v3f		diff = player->aabb.pos + player->aabb.dim / 2.f \
						- self_center;
	const float		dist = v3fmag(diff);

	self->rot[x] = atan2f(diff[z], diff[x]);
	if (dist > .15f)
	{
		if (dist < 4.5f)
		{
			self->vel = v3fnorm((t_v3f){diff[x], 0.f, diff[z]}, dt * .6f);
			self->aabb.pos[y] += (3.f - self->aabb.pos[y]) * dt * 2.f;
		}
		__attack(game, self, dt, dist);
		if (map_get(&game->map, v3ftoi(self_center)) == cell_wall)
		{
			if (self->dir[x] >= .01f)
				_enemy_fish_part(game, self, diff);
			self->dir[x] += dt;
			self->vel = v3fnorm((t_v3f){diff[x], 0.f, diff[z]}, dt * .2f);
		}
	}
	self->dir[y] += dt;
}

static void	_enemy_fish_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;
	t_v3f		slide;

	slide = v3frot((t_v3f){.3f, 0.f, 0.f}, self->rot);
	trans.rotation = (t_v2f){self->rot[x], 0.f};
	trans.resize = (t_v3f){fabsf(sinf(self->dir[y] * 2.f)) \
		* 0.1f + 0.25f, 0.25f, 0.25f};
	trans.translation = self->aabb.pos + self->aabb.dim / 2.f;
	trans.translation[y] = self->aabb.pos[y];
	mesh_put(game->eng, &game->cam, trans, &game->models[6]);
}

t_entity	*e_enemy_fish_add(t_data *const game, t_v3f const pos, t_v2f rot)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_enemy_fish_update;
	ent->display = &_enemy_fish_display;
	ent->dir = (t_v3f){0.f};
	ent->max_health = 15.f;
	ent->health = ent->max_health;
	ent->rot = rot;
	ent->dir[y] = ft_rand(0.f, 10.f);
	ent->aabb = (t_aabb){pos - (t_v3f){.15f, .0f, .15f}, \
		{.3f, .3f, .3f}, AABB_MOVABLE};
	ent->mesh = &game->models[6];
	ent->type = ENTITY_ENNEMY_FISH;
	return (ent);
}
