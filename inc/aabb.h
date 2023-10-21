/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 21:01:42 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/21 06:58:14 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AABB_H
# define AABB_H

# include "map.h"
# include "camera.h"

typedef struct s_aabb	t_aabb;

enum e_aabb_type
{
	AABB_NONE,
	AABB_MOVABLE,
	AABB_IMMOVABLE,
};

struct s_aabb
{
	t_v3f	pos;
	t_v3f	dim;
	int		type;
};

t_v3f	ray_box_intersection(
			t_v3f const p1,
			t_v3f const p2,
			t_v3f const b_min,
			t_v3f const b_max);

int		is_aabb_in_aabb(t_aabb const box1, t_aabb const box2);
t_v3f	aabb_solve(
			t_aabb *const box1,
			t_v3f *const vel1,
			t_aabb *const box2);

void	collision_ent(t_vector *const entities, t_map *const map);

#endif
