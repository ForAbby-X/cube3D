/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 18:18:43 by vmuller           #+#    #+#             */
/*   Updated: 2023/10/19 20:18:19 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "vec3.h"
# include "engine.h"

typedef struct s_camera	t_camera;

struct s_camera
{
	t_v3f		pos;
	t_v2f		rot;
	float		fov;
	float		fov_ratio;
	float		screen_dist;
	t_v3f		fru_near;
	t_v3f		fru_far;
	int			fog;
	t_color		fog_color;
	float		fog_distance;
	t_sprite	*surface;
	float		*depth_buffer;
	t_vector	poly_raw;
	t_vector	poly_clip;
};

t_camera	camera_create(t_engine *const eng, t_v2i const surface);
void		camera_destroy(t_engine *const eng, t_camera *const cam);

void		camera_update(t_camera *const cam);

void		camera_set_depth(
				t_camera *const cam,
				t_v2i const pos,
				float const depth);
float		camera_get_depth(t_camera *const cam, t_v2i const pos);

void		shader_apply_depth(t_camera *const cam);

#endif
