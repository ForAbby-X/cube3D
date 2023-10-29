/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 20:47:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/29 17:38:17 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aabb.h"
#include "entity/entity.h"

static inline float	__ray_box_intersection(
			t_v3f const ray_pos,
			t_v3f const ray_dir,
			t_v3f const box_pos,
			t_v3f const box_dim)
{
	t_v3f const	inv = 1.f / ray_dir;
	t_v3f const	t1 = (box_pos - ray_pos) * inv;
	t_v3f const	t2 = ((box_pos + box_dim) - ray_pos) * inv;
	t_v2f		tt;

	if (ray_dir[x] == 0.f && ray_dir[y] == 0.f && ray_dir[z] == 0.f)
		return (1.f);
	tt = (t_v2f){-INFINITY, INFINITY};
	if (ray_dir[x] != 0)
	{
		tt[0] = fmaxf(tt[0], fminf(t1[x], t2[x]));
		tt[1] = fminf(tt[1], fmaxf(t1[x], t2[x]));
	}
	if (ray_dir[y] != 0)
	{
		tt[0] = fmaxf(tt[0], fminf(t1[y], t2[y]));
		tt[1] = fminf(tt[1], fmaxf(t1[y], t2[y]));
	}
	if (ray_dir[z] != 0)
	{
		tt[0] = fmaxf(tt[0], fminf(t1[z], t2[z]));
		tt[1] = fminf(tt[1], fmaxf(t1[z], t2[z]));
	}
	if (tt[1] >= tt[0] && tt[0] >= 0.f && tt[0] < 1.f && tt[1] >= 0.f)
	{
		printf("===--~\nCollision for T : %f\n", tt[0]);
		return (tt[0]);
	}
	else
		return (1.f);
}

int	is_ray_box_intersecting(
			t_v3f const ray_pos,
			t_v3f const ray_dir,
			t_v3f const box_pos,
			t_v3f const box_dim)
{
	t_v3f const	inv = 1.f / ray_dir;
	t_v3f const	t1 = (box_pos - ray_pos) * inv;
	t_v3f const	t2 = (box_pos + box_dim - ray_pos) * inv;
	t_v2f		tt;

	tt = (t_v2f){-INFINITY, INFINITY};
	if (ray_dir[x] == 0.f && ray_dir[y] == 0.f && ray_dir[z] == 0.f)
		return (0);
	if (ray_dir[x] != 0.f)
	{
		tt[0] = fmaxf(tt[0], fminf(t1[x], t2[x]));
		tt[1] = fminf(tt[1], fmaxf(t1[x], t2[x]));
	}
	if (ray_dir[y] != 0.f)
	{
		tt[0] = fmaxf(tt[0], fminf(t1[y], t2[y]));
		tt[1] = fminf(tt[1], fmaxf(t1[y], t2[y]));
	}
	if (ray_dir[z] != 0.f)
	{
		tt[0] = fmaxf(tt[0], fminf(t1[z], t2[z]));
		tt[1] = fminf(tt[1], fmaxf(t1[z], t2[z]));
	}
	return (tt[1] >= tt[0] && tt[0] >= 0.f && tt[0] < 1.f && tt[1] >= 0.f);
}

t_v3f	ray_box_intersection(
			t_v3f const ray_pos,
			t_v3f const ray_dir,
			t_v3f const box_pos,
			t_v3f const box_dim)
{
	float	t;

	t = __ray_box_intersection(ray_pos, ray_dir, box_pos, box_dim);
	return (ray_pos + ray_dir * t);
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

int	is_aabb_entering_aabb(
		t_aabb const *const box1,
		t_v3f const dir1,
		t_aabb const *const box2)
{
	t_v3f const	box_pos = box2->pos - box1->dim / 2.f;
	t_v3f const	box_dim = box1->dim + box2->dim;
	t_v3f const	center = box1->pos + box1->dim / 2.f;
	int			result;

	result = is_ray_box_intersecting(center, dir1, box_pos, box_dim);
	return (result);
}

static inline t_v3f	__get_norm(t_v3f const point, t_v3f const center)
{
	t_v3f const	diff = point - center;
	t_v3f const	abs = {fabsf(diff[x]), fabsf(diff[y]), fabsf(diff[z])};

	if (abs[x] > abs[y] && abs[x] > abs[z])
		return ((t_v3f){1.f, 0.f, 0.f});
	else if (abs[y] > abs[x] && abs[y] > abs[z])
		return ((t_v3f){0.f, 1.f, 0.f});
	return ((t_v3f){0.f, 0.f, 1.f});
}

t_v3f	aabb_solve(
			t_aabb const *const box1,
			t_v3f const *const vel1,
			t_aabb const *const box2)
{
	t_v3f const	box_pos = box2->pos - box1->dim / 2.f;
	t_v3f const	box_dim = box1->dim + box2->dim + 0.001f;
	t_v3f const	center = box1->pos + box1->dim / 2.0f;
	t_v3f		norm;
	t_v3f		off;

	off = ray_box_intersection(center, *vel1, box_pos, box_dim);
	norm = __get_norm(off, box2->pos + box2->dim / 2.f);
	off = (center + *vel1) - off;
	return (off * norm);
}

static inline void	__handle_collision(
	t_aabb *const box1,
	t_v3f *const vel1,
	t_aabb *const box2,
	t_v3f *const vel2)
{
	t_v3f	to_move;

	to_move = aabb_solve(box1, vel1, box2);
	printf("NORM : %f %f %f\n", to_move[x], to_move[y], to_move[z]);
	if (box1->type == AABB_IMMOVABLE && box2->type == AABB_MOVABLE)
		*vel2 += to_move;
	else if (box1->type == AABB_MOVABLE && box2->type == AABB_IMMOVABLE)
	{
		printf("VEL BEFORE : %f, %f, %f\n", (*vel1)[x], (*vel1)[y], (*vel1)[z]);
		*vel1 -= to_move;
		printf("VEL AFTER : %f, %f, %f\n", (*vel1)[x], (*vel1)[y], (*vel1)[z]);
	}
	else if (box1->type == AABB_MOVABLE && box2->type == AABB_MOVABLE)
	{
		*vel1 -= to_move / 2.0f;
		*vel2 += to_move / 2.0f;
	}
}

static inline void	__block_collision(
		t_map *const map,
		t_aabb *const box,
		t_v3f *const vel)
{
	t_v3i			pos;
	t_v3i			block;
	t_aabb			block_box;
	t_v3i const		player_pos = v3ftoi(box->pos + box->dim / 2.f + *vel);

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
				if (is_aabb_entering_aabb(box, *vel, &block_box))
					__handle_collision(box, vel, &block_box, &(t_v3f){0});
			}
		}
	}
}

static inline void	__ent_loop(
						t_vector *const entities,
						t_entity *const self,
						float const dt)
{
	t_entity	*ent;
	t_length	len;
	t_aabb		next;
	// t_v3f		diff;

	(void)dt;
	ent = entities->data;
	len = entities->size;
	while (len > 0)
	{
		next = self->aabb;
		next.pos += self->vel;
		// if (self != ent && (ent->aabb.type & self->aabb.type) == AABB_MOVABLE
		// 	&& is_aabb_in_aabb(self->aabb, ent->aabb))
		// {
		// 	diff = (self->aabb.pos + self->aabb.dim / 2.f)
		// 		- (ent->aabb.pos + ent->aabb.dim / 2.f);
		// 	diff = 100.f / diff;
		// 	diff[y] = 0.f;
		// 	self->vel += diff / 2.f * dt;
		// 	ent->vel -= diff / 2.f * dt;
		// }
		// if (self != ent && ent->aabb.type != AABB_NONE
		// 	&& is_aabb_entering_aabb(&self->aabb, self->vel, &ent->aabb))
		// 	__handle_collision(&self->aabb, &self->vel, &ent->aabb, &ent->vel);
		ent++;
		len--;
	}
}

void	collision_ent(
			t_vector *const entities,
			t_map *const map,
			float const dt)
{
	t_entity	*ent;
	t_length	len;

	ent = entities->data;
	len = entities->size;
	while (len > 0)
	{
		if (ent->aabb.type != AABB_NONE)
		{
			__ent_loop(entities, ent, dt);
			__block_collision(map, &ent->aabb, &ent->vel);
		}
		ent->aabb.pos += ent->vel;
		ent->vel = (t_v3f){0};
		ent++;
		len--;
	}
}
