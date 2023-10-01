/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 19:41:10 by vmuller           #+#    #+#             */
/*   Updated: 2023/09/30 23:44:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTER_H
# define RAYCASTER_H

# include "game.h"
# include "model.h"
# include "cast_ray.h"

typedef enum e_cell_side
{
	north,
	south,
	west,
	east,
	up,
	down
}	t_cell_side;

void		get_tex_pos(t_ray *const ray, t_v2f *const tex_pos);
t_cell_side	get_real_side(t_ray *const ray);
t_color		ray_to_pixel(
				t_map *const map,
				t_ray *const ray,
				long reflections);

void		ray_render(
				t_engine *const eng,
				t_map *const map,
				t_camera *const cam);
t_color		ray_reflection(
				t_map *const map,
				t_ray *const ray,
				t_color const color,
				long reflections);

#endif
