/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:05:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/04 16:18:39 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/generic.h"

t_entity	entity_create(t_data *const game, t_v3f const pos)
{
	t_entity	ent;

	ent = (t_entity){0};
	ent.update = &entity_update;
	ent.display = &entity_display;
	ent.destroy = &entity_destroy;
	ent.data = NULL;
	ent.pos = pos;
	ent.rot = (t_v2f){0};
	ent.mesh = &game->models[0];
	ent.alive = 0.0f;
	ent.uuid = 0ul;
	ent.type = ENTITY_GENERIC;
	return (ent);
}

void	entity_update(t_entity *const self, t_data *const game, float const dt)
{
	static t_v3f	last_pos;
	static float	energy_vel;

	self->alive += dt;
	energy_vel += v3fmag(game->cam.pos - last_pos);
	self->pos = game->cam.pos + v3froty(
			v3frotz((t_v3f){
				0.2f, -0.15f + sinf(energy_vel * 7.f) / 100.f,
				0.2f},
				self->rot[y]), self->rot[x]);
	self->rot = game->cam.rot;
	last_pos = game->cam.pos;
}

void	entity_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;

	trans.rotation = self->rot;
	trans.rotation[y] -= 0.8f;
	trans.resize = (t_v3f){
		0.12f,
		0.12f,
		0.12f};
	trans.translation = self->pos;
	mesh_put(game->eng, &game->cam, trans, self->mesh);
}

void	entity_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

