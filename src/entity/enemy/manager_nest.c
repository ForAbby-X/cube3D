/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_nest.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 03:09:47 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/14 14:12:49 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "particle/particle.h"

static inline void	__particle(t_data *const game, t_entity *const self)
{
	t_particle	*part;
	t_v3f		dir;
	int			num;

	num = -1;
	while (++num < 5)
	{
		dir = v3frot((t_v3f){ft_rand(0.f, .3f)},
				(t_v2f){ft_rand(-M_PI, M_PI), 0.f});
		dir[y] = ft_rand(-.5f, .2f);
		part = particle_add(game, self->aabb.pos + self->aabb.dim / 2.f
				+ dir + (t_v3f){0.f, 0.2f, 0.0f}, (t_v3f){0.f, 0.3f, 0.0f});
		if (part == NULL)
			return ;
		part->force = 1.2f;
		part->spr = game->sprites[9];
		part->death_time = ft_rand(2.5f, 2.9f);
	}
}

static inline void	__spawn(t_data *const game, t_entity *const self)
{
	t_v3f		pos;

	pos[y] = 0.0f;
	if (rand() & 1)
	{
		pos[x] = ft_rand(-0.5f, 0.5f);
		if (rand() & 1)
			pos[z] = -0.5f;
		else
			pos[z] = 0.5f;
	}
	else
	{
		pos[z] = ft_rand(-0.5f, 0.5f);
		if (rand() & 1)
			pos[x] = -0.5f;
		else
			pos[x] = 0.5f;
	}
	pos[x] += self->aabb.dim[x] / 2.f;
	pos[z] += self->aabb.dim[z] / 2.f;
	pos += self->aabb.pos;
	e_enemy_fish_add(game, pos, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
}

static void	_enemy_nest_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	t_entity *const	player = game->entities.data;
	const t_v3f		self_center = self->aabb.pos + self->aabb.dim / 2.f;
	const t_v3f		player_center = player->aabb.pos + player->aabb.dim / 2.f;
	const t_v3f		diff = player_center - self_center;
	const float		dist = v3fmag(diff);

	if (self->dir[x] >= 1.2f)
	{
		__particle(game, self);
		self->dir[x] = 0.0f;
	}
	self->dir[x] += dt;
	self->dir[y] += dt;
	if (dist < 5.f && self->dir[y] >= 5.f)
	{
		self->dir[y] = 0.0f;
		__spawn(game, self);
	}
}

static void	_enemy_nest_display(t_entity *const self, t_data *const game)
{
	t_transform	transform;

	transform.resize = (t_v3f){0.35f, 0.35f, 0.35f}
		+ sinf(self->time_alive) * .015f;
	transform.rotation = self->rot;
	transform.translation = self->aabb.pos + (t_v3f){.3f, 0.f, .3f};
	mesh_put(game->eng, &game->cam, transform, &game->models[15]);
}

t_entity	*e_enemy_nest_add(t_data *const game, t_v3f const pos)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_enemy_nest_update;
	ent->display = &_enemy_nest_display;
	ent->dir = (t_v3f){0.f};
	ent->rot = (t_v2f){ft_rand(-M_PI, M_PI), 0.f};
	ent->max_health = 120.f;
	ent->health = ent->max_health;
	ent->aabb = (t_aabb){pos - (t_v3f){.3f, 0.f, .3f}, {.6f, .6f, .6f},
		AABB_IMMOVABLE};
	ent->mesh = &game->models[0];
	ent->type = ENTITY_ENNEMY_NEST;
	return (ent);
}
