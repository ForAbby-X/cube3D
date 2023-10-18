/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 16:33:08 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/18 15:51:14 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"

static inline float	__diff_angle(float a, float b)
{
	return (acosf(cosf(a) * cosf(b) + sinf(a) * sinf(b)));
}

static void	_mimic_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	t_v3f		diff;
	float		r_diff;
	float		mag;
	float const	value = fabsf(sinf(self->time_alive * 4.f)) * 1.2f;

	diff = game->cam.pos - self->pos;
	diff[y] = 0.f;
	mag = diff[x] * diff[x] + diff[z] * diff[z];
	diff = v3fnorm(diff, 1.f);

	r_diff = __diff_angle(atan2(diff[z], diff[x]), self->rot[x]);

	if (mag > 1.f)
	{
		self->rot[x] += r_diff * dt * 4.f;
		self->pos += diff * dt * value;
	}
	else
	{
		self->rot[x] += (r_diff + M_PI) * dt * 4.f;
		self->pos -= diff * dt * value * 1.2f;
	}
}

static void	_mimic_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;
	float const	value = fabsf(sinf(self->time_alive * 4.f)) * 0.3f;

	trans.rotation = self->rot;
	trans.rotation[y] = value;
	trans.resize = (t_v3f){0.5f, 0.5f, 0.5f};
	trans.translation = self->pos + (t_v3f){0.f, value, 0.f};
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
	ent->type = ENTITY_MIMIC;
	return (ent);
}
