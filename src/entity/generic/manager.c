/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:05:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/05 19:11:11 by alde-fre         ###   ########.fr       */
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
	static float	click_energy;

	self->alive += dt;
	energy_vel += v3fmag(game->cam.pos - last_pos);
	self->pos = game->cam.pos + v3froty(
			v3frotz((t_v3f){
				0.2f,
				-0.15f + sinf(energy_vel * 7.f) / 150.f,
				0.2f},
				self->rot[y]), self->rot[x]);
	self->rot = game->cam.rot;
	self->rot[y] -= click_energy;
	last_pos = game->cam.pos;
	if (ft_mouse(game->eng, 1).pressed)
		click_energy = 0.3f;
	click_energy -= dt / 1.2f;
	click_energy = fmaxf(click_energy, 0.0f);
}

void	entity_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;

	trans.rotation = self->rot;
	trans.rotation[y] -= 0.8f;
	trans.rotation[x] += sinf(self->alive * M_PI / 6.f) * 0.0125f;
	trans.rotation[y] += cosf(self->alive * M_PI / 4.f) * 0.05f;
	trans.resize = (t_v3f){0.12f, 0.12f, 0.12f} / 100.f;
	trans.translation = self->pos;
	mesh_put(game->eng, &game->cam, trans, self->mesh);
}

void	entity_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

