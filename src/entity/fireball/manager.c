/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:05:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/13 11:29:53 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "particle/particle.h"

static void	_fireball_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	self->pos += self->dir * 4.f * dt;
	self->dead = (self->time_alive >= 2.f \
		|| map_get(&game->map, v3ftoi(self->pos)));
	if (self->dead)
		p_fire_explosion_add(game, self->pos);
	self->time_alive += dt;
}

static void	_fireball_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;

	trans.rotation[x] = self->rot[x] + sinf(self->time_alive * 4.f) * 0.2f;
	trans.rotation[y] = self->rot[y] + cosf(self->time_alive * 5.f) * 0.2f;
	trans.resize = (t_v3f){0.3f, 0.3f, 0.3f};
	trans.translation = self->pos;
	mesh_put(game->eng, &game->cam, trans, self->mesh);
	if (self->time_alive >= 0.0625f)
	{
		p_fire_add(game, self->pos, self->dir * 0.5f);
		self->time_alive = 0.f;
	}
}

static void	_fireball_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

t_entity	*e_fireball_add(
				t_data *const game,
				t_v3f const pos,
				t_v2f const rot)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_fireball_update;
	ent->display = &_fireball_display;
	ent->destroy = &_fireball_destroy;
	ent->dir = v3froty(v3frotz((t_v3f){1.f}, rot[y]), rot[x]);
	ent->rot = rot;
	ent->mesh = &game->models[1];
	ent->type = ENTITY_FIREBALL;
	return (ent);
}
