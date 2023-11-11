/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:33:08 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/11 10:47:16 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "gameplay_utils.h"

static void	_mimic_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	(void)self;
	(void)game;
	(void)dt;
}

static void	_mimic_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;

	trans.rotation = self->rot;
	trans.resize = (t_v3f){0.5f, 0.5f, 0.5f};
	trans.translation = self->aabb.pos + (t_v3f){0.125f, 0.f, 0.125f};
	mesh_put(game->eng, &game->cam, trans, &game->models[5]);
}

static void	_mimic_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

t_entity	*e_mimic_add(
				t_data *const game,
				t_v3f const pos,
				t_v2f const rot)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_mimic_update;
	ent->display = &_mimic_display;
	ent->destroy = &_mimic_destroy;
	ent->rot = rot;
	ent->aabb = (t_aabb){pos - (t_v3f){0.125f, 0.0f, 0.125f},
	{0.25f, 0.45f, 0.25f}, AABB_IMMOVABLE};
	ent->type = ENTITY_MIMIC;
	return (ent);
}
