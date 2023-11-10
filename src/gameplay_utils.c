/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:40:20 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/10 03:56:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gameplay_utils.h"

int	ray_hit_box(
		t_map *const map,
		t_v3f const pos,
		t_aabb const *const box,
		float const dist)
{
	t_v3f const	box_center = box->pos + box->dim / 2.f;
	t_v3f const	diff = box_center - pos;
	float const	len = v3fmag(diff);
	float		cdist;

	return (ray_box_intersection(pos, diff / len * dist, *box, &cdist)
		&& cast_ray(map, pos, diff, 99999.f).dist > dist);
}
