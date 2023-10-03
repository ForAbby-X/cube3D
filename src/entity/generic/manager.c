/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:05:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/03 15:21:59 by alde-fre         ###   ########.fr       */
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
	t_v3f	diff;

	self->alive += dt;
	diff = game->cam.pos - self->pos;
	diff[y] = 0.f;
	self->rot[x] = atan2(diff[z], diff[x]);
	self->pos += v3fnorm(diff, 0.35f) * dt;
	// self->rot[y] = atan2(diff[y], ft_v2fmag((t_v2f){diff[z], diff[x]}));
}

void	entity_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;

	trans.rotation = self->rot;
	trans.resize = (t_v3f){1.f + sinf(self->alive * 8.f) / 15.f, 1.f + sinf(self->alive * 5.f) / 10.f, 1.f + sinf(self->alive * 8.f + 0.3f) / 15.f} / 200.f;
	trans.translation = self->pos;
	mesh_put(game->eng, &game->cam, trans, self->mesh);
}

void	entity_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

