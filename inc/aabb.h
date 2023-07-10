/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 21:01:42 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/10 15:18:06 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AABB_H
# define AABB_H

# include "map.h"
# include "camera.h"

typedef struct s_aabb	t_aabb;

struct s_aabb
{
	t_v3f	pos;
	t_v3f	dim;
};

int		is_aabb_in_aabb(
			t_aabb const box1,
			t_aabb const box2);
void	aabb_solve(
			t_aabb *const box1,
			t_aabb *const box2);

void	player_collision(
			t_map *const map,
			t_aabb *const box);

#endif
