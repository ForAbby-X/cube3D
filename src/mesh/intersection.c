/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 15:35:30 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/23 08:10:04 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

t_vert	inter_ray_pol(
			t_v3f const plane,
			t_xyz const axis,
			t_vert const p1,
			t_vert const p2)
{
	t_v3f const	diff_r = p2.point - p1.point;
	t_v2f const	diff_uv = p2.uv - p1.uv;
	float const	t = (plane[axis] - p1.point[axis]) / diff_r[axis];

	if (t > 1.0f)
		return ((t_vert){p2.point, p2.uv});
	if (t < 0.0f)
		return ((t_vert){p1.point, p1.uv});
	return ((t_vert){p1.point + diff_r * t, p1.uv + diff_uv * t});
}
