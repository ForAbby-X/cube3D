/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 20:47:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/21 08:07:08 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aabb.h"
#include "entity/entity.h"

t_v3f	ray_box_intersection(
			t_v3f const p1,
			t_v3f const p2,
			t_v3f const b_min,
			t_v3f const b_max)
{
	t_v3f const	diff_r = p2 - p1;
	t_v3f const	inv = 1.f / diff_r;
	t_v3f const	t1 = (b_min - p1) * inv;
	t_v3f const	t2 = (b_max - p1) * inv;
	t_v2f		tt;

	tt = (t_v2f){0.0f, INFINITY};
	tt[0] = fminf(fmaxf(t1[x], tt[0]), fmaxf(t2[x], tt[0]));
	tt[1] = fmaxf(fminf(t1[x], tt[1]), fminf(t2[x], tt[1]));
	tt[0] = fminf(fmaxf(t1[y], tt[0]), fmaxf(t2[y], tt[0]));
	tt[1] = fmaxf(fminf(t1[y], tt[1]), fminf(t2[y], tt[1]));
	tt[0] = fminf(fmaxf(t1[z], tt[0]), fmaxf(t2[z], tt[0]));
	tt[1] = fmaxf(fminf(t1[z], tt[1]), fminf(t2[z], tt[1]));
	if (tt[0] <= tt[1])
		return (p1 + diff_r * tt[0]);
	return (p2);
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

t_v3f	aabb_solve(
			t_aabb *const box1,
			t_v3f *const vel1,
			t_aabb *const box2)
{
	t_v3f const	box_min = box2->pos - box1->dim / 2.0f
		- (t_v3f){.01f, .01f, .01f};
	t_v3f const	box_max = box2->pos + box1->dim + box2->dim;
	t_v3f const	center = box1->pos + box1->dim / 2.0f;
	t_v3f		norm;
	t_v3f		off;

	off = ray_box_intersection(center, center + *vel1, box_min, box_max);
	norm = off - (box2->pos + box2->dim / 2.0f);
	float max = fmaxf(fmaxf(fabsf(norm[x]), fabsf(norm[y])), fabsf(norm[z]));
	off = center + *vel1 - off;
	if (fabsf(norm[x]) == max)
		return ((t_v3f){off[x], 0.f, 0.f});
	if (fabsf(norm[y]) == max)
		return ((t_v3f){0.f, off[y], 0.f});
	return ((t_v3f){0.f, 0.f, off[z]});
}

static inline void	__handle_collision(
	t_aabb *const box1,
	t_v3f *const vel1,
	t_aabb *const box2,
	t_v3f *const vel2)
{
	t_v3f const	to_move = aabb_solve(box1, vel1, box2);

	if (box1->type == AABB_IMMOVABLE && box2->type == AABB_MOVABLE)
		*vel2 += to_move;
	else if (box1->type == AABB_MOVABLE && box2->type == AABB_IMMOVABLE)
		*vel1 -= to_move;
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
	t_aabb const	player = {box->pos + *vel, box->dim, box->type};

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
				if (!map_get(map, block))
					continue ;
				block_box = (t_aabb){{block[x], block[y], block[z]},
				{1.0f, 1.0f, 1.0f}, AABB_IMMOVABLE};
				if (is_aabb_in_aabb(player, block_box))
					__handle_collision(box, vel, &block_box, &(t_v3f){});
			}
		}
	}
}

static inline void	__ent_loop(t_vector *const entities, t_entity *const self)
{
	t_entity	*ent;
	t_length	len;
	t_aabb		next;

	ent = entities->data;
	len = entities->size;
	while (len > 0)
	{
		next = self->aabb;
		next.pos += self->vel;
		if (ent->aabb.type != AABB_NONE && self != ent
			&& is_aabb_in_aabb(next, ent->aabb))
		{
			__handle_collision(&self->aabb, &self->vel, &ent->aabb, &ent->vel);
		}
		ent++;
		len--;
	}
}

void	collision_ent(t_vector *const entities, t_map *const map)
{
	t_entity	*ent;
	t_length	len;

	ent = entities->data;
	len = entities->size;
	while (len > 0)
	{
		if (ent->aabb.type != AABB_NONE)
		{
			__block_collision(map, &ent->aabb, &ent->vel);
			__ent_loop(entities, ent);
		}
		// printf("ENT vel [%u][%f, %f, %f]\n", entities->size - len, ent->vel[x], ent->vel[y], ent->vel[z]);
		ent->aabb.pos += ent->vel;
		ent->vel = (t_v3f){0};
		ent++;
		len--;
	}
}

// void	collision(
// 	t_map *const map,
// 	t_aabb *const box)
// {
// 	t_v3i		pos;
// 	t_v3i		block;
// 	t_aabb		block_box;
// 	t_v3i const	player_pos = {box->pos[x], box->pos[y], box->pos[z]};

// 	pos[x] = -2;
// 	while (++pos[x] < 2)
// 	{
// 		pos[y] = -2;
// 		while (++pos[y] < 2)
// 		{
// 			pos[z] = -2;
// 			while (++pos[z] < 2)
// 			{
// 				block = player_pos + pos;
// 				if (map_get(map, block))
// 				{
// 					block_box = (t_aabb){{block[x], block[y], block[z]},
// 					{1.0f, 1.0f, 1.0f}};
// 					if (is_aabb_in_aabb(*box, block_box))
// 						__handle_collision(&block_box, box);
// 				}
// 			}
// 		}
// 	}
// }
