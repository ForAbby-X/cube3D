/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 20:47:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/07 08:00:38 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aabb.h"
#include "entity/entity.h"

int	ray_box_intersection(
			t_v3f const ray_pos,
			t_v3f const ray_dir,
			t_aabb const box,
			float *const t)
{
	t_v3f const	dir_inv = 1.f / ray_dir;
	t_v2f		tmm;
	t_v2f		tg;
	int			d;

	tmm = (t_v2f){0.0f, INFINITY};
	d = 0;
	while (d < 3)
	{
		tg[0] = (box.pos[d] - ray_pos[d]) * dir_inv[d];
		tg[1] = (box.pos[d] - ray_pos[d]
				+ (box.dim[d] - __FLT_EPSILON__)) * dir_inv[d];
		tmm[0] = fmin(fmax(tg[0], tmm[0]), fmax(tg[1], tmm[0]));
		tmm[1] = fmax(fmin(tg[0], tmm[1]), fmin(tg[1], tmm[1]));
		++d;
	}
	*t = tmm[0];
	return (tmm[0] < tmm[1] && tmm[1] >= 0.f && tmm[0] >= 0.f && tmm[0] <= 1.f);
}

int	is_aabb_in_aabb(t_aabb const box1, t_aabb const box2)
{
	if ((box2.pos[x] >= box1.pos[x] + box1.dim[x])
		|| (box2.pos[x] + box2.dim[x] <= box1.pos[x])
		|| (box2.pos[y] >= box1.pos[y] + box1.dim[y])
		|| (box2.pos[y] + box2.dim[y] <= box1.pos[y])
		|| (box2.pos[z] >= box1.pos[z] + box1.dim[z])
		|| (box2.pos[z] + box2.dim[z] <= box1.pos[z]))
		return (0);
	return (1);
}

static inline t_v3f	__vec_normal(
						t_v3f const pos,
						t_aabb const box,
						t_v3f const vel)
{
	t_v3f const	center = box.pos + box.dim / 2.f;
	t_v3f const	diff = (pos - center) / (box.dim / 2.f);
	t_v3f const	abs = {fabsf(diff[x]), fabsf(diff[y]), fabsf(diff[z])};
	t_v3f		norm;

	norm = (t_v3f){0.0f, 0.0f, 0.0f};
	if (abs[x] > abs[y] && abs[x] > abs[z])
		norm[x] = vel[x];
	else if (abs[y] > abs[x] && abs[y] > abs[z])
		norm[y] = vel[y];
	else /* if (abs[z] > abs[x] && abs[z] > abs[y]) */
		norm[z] = vel[z];
	return (norm);
}

int	aabb_solve(
		t_aabb const *const box1,
		t_v3f *const vel1,
		t_aabb const *const box2,
		t_v3f *const vel2)
{
	t_aabb const	box = {box2->pos - box1->dim / 2.f,
		box1->dim + box2->dim, 0};
	t_v3f const		center = box1->pos + box1->dim / 2.0f;
	float			t;
	t_v3f			off;

	if ((*vel1)[x] == 0.f && (*vel1)[y] == 0.f && (*vel1)[z] == 0.f)
		return (0);
	if (ray_box_intersection(center, *vel1, box, &t) == 0)
		return (0);
	off = __vec_normal(center + *vel1 * t, box, *vel1) * (1.f - t);
	if (box1->type == AABB_IMMOVABLE && box2->type == AABB_MOVABLE)
		*vel2 += off;
	else if (box1->type == AABB_MOVABLE && box2->type == AABB_IMMOVABLE)
		*vel1 -= off;
	else if (box1->type == AABB_MOVABLE && box2->type == AABB_MOVABLE)
	{
		off /= 2.f;
		*vel1 -= off;
		*vel2 += off;
	}
	return (1);
}

static inline void	__block_collision(
		t_map *const map,
		t_aabb *const box,
		t_v3f *const vel)
{
	t_v3i const		player_pos = v3ftoi(box->pos + box->dim / 2.f);
	t_v3i			pos;
	t_v3i			block;
	t_aabb			block_box;

	pos[x] = -2;
	while (++pos[x] < 2)
	{
		pos[y] = -2;
		while (++pos[y] < 2)
		{
			pos[z] = -2;
			while (++pos[z] < 2)
			{
				block = player_pos + pos;
				if (map_get(map, block) != cell_wall)
					continue ;
				block_box = (t_aabb){{block[x], block[y], block[z]},
				{1.f, 1.f, 1.f}, AABB_IMMOVABLE};
				aabb_solve(box, vel, &block_box, &(t_v3f){0});
			}
		}
	}
}

static inline void	__ent_loop(
						t_vector *const entities,
						t_entity *const self)
{
	t_entity	*ent;
	t_length	len;

	ent = entities->data;
	len = entities->size;
	while (len > 0)
	{
		if (self != ent && ent->aabb.type != AABB_NONE
			&& aabb_solve(&self->aabb, &self->vel, &ent->aabb, &ent->vel))
				self->collided = ent->type;
		ent++;
		len--;
	}
}

void	collision_ent(
			t_vector *const entities,
			t_map *const map)
{
	t_entity	*ent;
	t_length	len;

	ent = entities->data;
	len = entities->size;
	while (len > 0)
	{
		ent->collided = ENTITY_NONE;
		if (ent->aabb.type != AABB_NONE)
		{
			__ent_loop(entities, ent);
			__block_collision(map, &ent->aabb, &ent->vel);
		}
		ent->aabb.pos += ent->vel;
		ent->vel = (t_v3f){0};
		ent++;
		len--;
	}
}
