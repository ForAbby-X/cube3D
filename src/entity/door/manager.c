/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:33:08 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/07 07:05:29 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "particle/particle.h"

static void	_door_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	t_v3f		center;
	t_particle	*part;

	if (self->dir[x] > 1.f)
	{
		if (self->dir[y] == 0.0f)
		{
			center = self->aabb.pos + self->aabb.dim / 2.f;
			center[y] = (int)(self->aabb.pos[y]) + 0.98f;
			if (self->rot[x] > M_PI_2)
				center[z] += ft_rand(-.5f, .5f);
			else
				center[x] += ft_rand(-.5f, .5f);
			part = particle_add(game, center, (t_v3f){0.f, ft_rand(-.3f, -.15f)});
			part->force = ft_rand(0.5f, 1.5f);
			part->spr = game->sprites[5];
			part->death_time = ft_rand(0.6f, 0.7f);
			self->dir[y] = .03f;
		}
		self->dir[y] = fmaxf(self->dir[y] - dt, 0.f);
		self->aabb.pos[y] += dt;
		self->dir[x] = fmaxf(self->dir[x] - dt, 0.0f);
	}
}

static void	_door_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;

	trans.rotation = self->rot;
	trans.resize = (t_v3f){1.0f, 1.0f, 1.0f};
	trans.translation = self->aabb.pos + self->aabb.dim / 2.f;
	mesh_put(game->eng, &game->cam, trans, &game->models[7]);
}

static void	_door_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

t_entity	*e_door_add(t_data *const game, t_v3f const pos, t_v2f const rot)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_door_update;
	ent->display = &_door_display;
	ent->destroy = &_door_destroy;
	ent->dir = (t_v3f){0};
	ent->rot = rot;
	ent->mesh = &game->models[7];
	ent->aabb = (t_aabb){pos, {1.0f, 1.0f, 1.0f}, AABB_IMMOVABLE};
	if (rot[x] > M_PI_2)
		ent->aabb.pos -= (t_v3f){0.1f, 0.0f, 0.5f};
	else
		ent->aabb.pos -= (t_v3f){0.5f, 0.0f, 0.1f};
	if (rot[x] > M_PI_2)
		ent->aabb.dim = (t_v3f){0.2f, 1.0f, 1.0f};
	else
		ent->aabb.dim = (t_v3f){1.0f, 1.0f, 0.2f};
	ent->type = ENTITY_DOOR;
	return (ent);
}
