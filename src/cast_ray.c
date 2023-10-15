/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 18:17:19 by vmuller           #+#    #+#             */
/*   Updated: 2023/10/15 14:02:54 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cast_ray.h"

t_v3f	inter_ray_plane(t_v3f plane, t_xyz axis, t_v3f p1, t_v3f p2)
{
	t_v3f const	diff_r = p2 - p1;
	float const	t = (plane[axis] - p1[axis]) / diff_r[axis];

	if (t > 1.0f)
		return (p2);
	if (t < 0.0f)
		return (p1);
	return (p1 + diff_r * t);
}

static inline void	__setup_ray_step_delta_dist(
	t_ray *const ray,
	t_xyz const d)
{
	if (ray->dir[d] != 0.0f)
		ray->delta_dist[d] = fabsf(1.0f / ray->dir[d]);
	else
		ray->delta_dist[d] = 1e30;
	if (ray->dir[d] < 0)
	{
		ray->step[d] = -1;
		ray->side_dist[d] = (ray->start[d] - ray->pos[d]) * ray->delta_dist[d];
	}
	else
	{
		ray->step[d] = 1;
		ray->side_dist[d] = (ray->pos[d] + 1.0f - ray->start[d])
			* ray->delta_dist[d];
	}
}

static inline void	__setup_ray(
	t_ray *const ray,
	t_v3f *const pos,
	t_v3f *const dir)
{
	ray->pos = (t_v3i){(*pos)[x], (*pos)[y], (*pos)[z]};
	ray->dir = *dir;
	ray->start = *pos;
	ray->side = x;
	__setup_ray_step_delta_dist(ray, x);
	__setup_ray_step_delta_dist(ray, y);
	__setup_ray_step_delta_dist(ray, z);
}

static inline void	__loop_ray(
	t_map *const map,
	t_ray *const ray,
	int max_dist)
{
	t_v3i	inc;
	int		flag;

	inc = (t_v3i){0, 0, 0};
	flag = 1;
	while (flag && max_dist)
	{
		inc[x] = ((ray->side_dist[x] <= ray->side_dist[y])
				&& (ray->side_dist[x] <= ray->side_dist[z]));
		inc[y] = ((ray->side_dist[y] <= ray->side_dist[x])
				&& (ray->side_dist[y] <= ray->side_dist[z]));
		inc[z] = ((ray->side_dist[z] <= ray->side_dist[x])
				&& (ray->side_dist[z] <= ray->side_dist[y]));
		ray->pos += inc * ray->step;
		ray->side_dist += (t_v3f){inc[x], inc[y], inc[z]} * ray->delta_dist;
		flag = (map_get(map, ray->pos) == (t_cell){0});
		--max_dist;
	}
	ray->side = inc[y] | (inc[z] << 1);
	ray->dist = ray->side_dist[ray->side] - ray->delta_dist[ray->side];
	ray->end = ray->start + ray->dist * ray->dir;
}

t_ray	cast_ray(
	t_map *const map,
	t_v3f *const pos,
	t_v3f *const dir,
	float const max_dist)
{
	t_ray	ray;

	__setup_ray(&ray, pos, dir);
	__loop_ray(map, &ray, max_dist);
	return (ray);
}
