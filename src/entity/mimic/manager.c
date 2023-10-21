/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:33:08 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/21 08:08:19 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"

// static inline float	__diff_angle(float a, float b)
// {
// 	return (acosf(cosf(a) * cosf(b) + sinf(a) * sinf(b)));
// }

static void	_mimic_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	(void)self;
	(void)game;
	(void)dt;
	// t_v3f		diff;
	// float		r_diff;
	// float		mag;
	// // float const	value = fabsf(sinf(self->time_alive * 4.f)) * 1.2f;

	// diff = game->cam.pos - self->aabb.pos;
	// diff[y] = 0.f;
	// mag = diff[x] * diff[x] + diff[z] * diff[z];
	// diff = v3fnorm(diff, 1.f);

	// r_diff = __diff_angle(atan2(diff[z], diff[x]), self->rot[x]);

	// if (mag > 1.f)
	// {
	// 	self->rot[x] += r_diff * dt * 4.f;
	// 	// self->vel = diff * value * dt;
	// }
	// else
	// {
	// 	self->rot[x] += (r_diff + M_PI) * dt * 4.f;
	// 	// self->vel = -diff * value * 1.2f * dt;
	// }
}

static void	_mimic_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;
	// float const	value = fabsf(sinf(self->time_alive * 4.f)) * 0.3f;

	trans.rotation = self->rot;
	trans.resize = (t_v3f){0.5f, 0.5f, 0.5f};
	trans.translation = self->aabb.pos + (t_v3f){0.25f, 0.f, 0.25f};
	mesh_put(game->eng, &game->cam, trans, self->mesh);
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
	ent->dir = v3froty(v3frotz((t_v3f){1.f}, rot[y]), rot[x]);
	ent->rot = rot;
	ent->mesh = &game->models[5];
	ent->aabb = (t_aabb){pos - (t_v3f){0.01f, 0.01f, 0.01f},
	{0.02f, 0.02f, 0.02f}, AABB_IMMOVABLE};
	ent->type = ENTITY_MIMIC;
	return (ent);
}
