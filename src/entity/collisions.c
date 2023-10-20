/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 20:47:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/20 08:07:36 by alde-fre         ###   ########.fr       */
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
	t_v3f const	t1 = (b_min - p1) / diff_r;
	t_v3f const	t2 = (b_max - p1) / diff_r;
	t_v2f		tt;

	tt[0] = fminf(t1[x], t2[x]);
	tt[1] = fmaxf(t1[x], t2[x]);
	tt[0] = fmaxf(tt[0], fminf(t1[y], t2[y]));
	tt[1] = fminf(tt[1], fmaxf(t1[y], t2[y]));
	tt[0] = fmaxf(tt[0], fminf(t1[z], t2[z]));
	tt[1] = fminf(tt[1], fmaxf(t1[z], t2[z]));
	if (tt[0] > tt[1])
		return (p2);
	return (p1 + diff_r * tt[tt[0] < 0.0f]);
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

t_v3f	aabb_solve(t_aabb *const box1, t_aabb *const box2)
{
	t_aabb const	target = {box2->pos - box1->dim / 2.0f,
		box1->dim + box2->dim, 1};
	t_v3f const		center = box1->pos + box1->dim / 2.0f;
	t_v3f const		center2 = box2->pos + box2->dim / 2.0f;
	t_v3f			off;

	off = ray_box_intersection(center2, center,
			target.pos, target.pos + target.dim);
	return (box2->pos - (off - box1->dim / 2.f));
}

static inline void	__handle_collision(
	t_aabb *const box1,
	t_aabb *const box2)
{
	t_v3f const	to_move = aabb_solve(box1, box2);

	if (box1->type == AABB_IMMOVABLE && box2->type == AABB_MOVABLE)
		box2->pos -= to_move;
	else if (box1->type == AABB_MOVABLE && box2->type == AABB_IMMOVABLE)
		box1->pos += to_move;
	else if (box1->type == AABB_MOVABLE && box2->type == AABB_MOVABLE)
	{
		box1->pos += to_move / 2.0f;
		box2->pos -= to_move / 2.0f;
	}
}

static inline void	__block_collision(t_map *const map, t_aabb *const box)
{
	t_v3i		pos;
	t_v3i		block;
	t_aabb		block_box;
	t_v3i const	player_pos = {box->pos[x], box->pos[y], box->pos[z]};

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
				if (map_get(map, block))
				{
					block_box = (t_aabb){{block[x], block[y], block[z]},
					{1.0f, 1.0f, 1.0f}, AABB_IMMOVABLE};
					if (is_aabb_in_aabb(*box, block_box))
						__handle_collision(&block_box, box);
				}
			}
		}
	}
}

static inline void	__ent_loop(t_vector *const entities, t_entity *const self)
{
	t_entity	*ent;
	t_length	len;

	ent = entities->data;
	len = entities->size;
	while (len > 0)
	{
		if (ent->aabb.type != AABB_NONE && self != ent
			&& is_aabb_in_aabb(self->aabb, ent->aabb))
			__handle_collision(&ent->aabb, &self->aabb);
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
			__ent_loop(entities, ent);
			__block_collision(map, &ent->aabb);
		}
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
