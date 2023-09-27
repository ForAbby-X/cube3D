/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 20:47:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/08 17:33:34 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aabb.h"

//  if((box2.x >= box1.x + box1.w)      // trop à droite
//     || (box2.x + box2.w <= box1.x) // trop à gauche
//     || (box2.y >= box1.y + box1.h) // trop en bas
//     || (box2.y + box2.h <= box1.y))  // trop en haut
//           return false; 
//    else
//           return true; 

int	is_aabb_in_aabb(
		t_aabb const box1,
		t_aabb const box2)
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

void	aabb_solve(
			t_aabb *const box1,
			t_aabb *const box2)
{
	t_v3f const	cent1 = box1->pos + box1->dim / 2.0f;
	t_v3f const	cent2 = box2->pos + box2->dim / 2.0f;
	t_v3f const	diff = cent2 - cent1;
	t_v3f const	abs = {fabsf(diff[x]), fabsf(diff[y]), fabsf(diff[z])};
	int			side;

	if (abs[x] > abs[y] && abs[x] > abs[z])
		side = 0;
	else if (abs[y] > abs[x] && abs[y] > abs[z])
		side = 1;
	else
		side = 2;
	if (cent2[side] > cent1[side])
		box2->pos[side] = box1->pos[side] + box1->dim[side];
	else
		box2->pos[side] = box1->pos[side] - (box2->dim[side]);
}

static inline void	__handle_collision(
	t_aabb *const block,
	t_aabb *const player)
{
	aabb_solve(block, player);
}

void	player_collision(
	t_map *const map,
	t_aabb *const box)
{
	t_v3i		pos;
	t_v3i		block;
	t_aabb		block_box;
	t_v3i const	player_pos = {box->pos[x], box->pos[y], box->pos[z]};

	pos[x] = -1;
	while (pos[x] < 2)
	{
		pos[y] = -1;
		while (pos[y] < 2)
		{
			pos[z] = -1;
			while (pos[z] < 2)
			{
				block = player_pos + pos;
				if (map_get(map, block))
				{
					block_box = (t_aabb){(t_v3f){block[x], block[y], block[z]},
						(t_v3f){1.0f, 1.0f, 1.0f}};
					if (is_aabb_in_aabb(*box, block_box))
						__handle_collision(&block_box, box);
				}
				pos[z]++;
			}
			pos[y]++;
		}
		pos[x]++;
	}
}
