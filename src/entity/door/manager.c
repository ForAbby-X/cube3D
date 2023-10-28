/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:33:08 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/28 16:19:23 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"

static void	_door_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	(void)self;
	(void)game;
	(void)dt;
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
