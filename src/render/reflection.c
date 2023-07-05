/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 03:09:00 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/05 16:13:47 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

t_color	ray_reflection(
	t_map *const map,
	t_ray *const ray,
	t_color const color,
	long const reflection)
{
	t_v3f	new_dir;
	t_ray	new_ray;
	t_color	new_color;
	float	value;

	new_dir = ray->dir;
	new_dir[ray->side] = -new_dir[ray->side];
	new_ray = cast_ray(map, &ray->end, &new_dir, 20);
	new_ray.dist -= ray->dist;
	new_color = ray_to_pixel(map, &new_ray, reflection);
	value = (float)color.a / 255.0f;
	return (ft_color_inter(new_color, color, value));
}
