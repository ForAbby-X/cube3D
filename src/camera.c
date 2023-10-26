/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 10:32:25 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/26 13:44:27 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "model.h"

t_camera	camera_create(t_engine *const eng, t_v2i const surface)
{
	t_camera	cam;

	cam.surface = ft_sprite(eng, surface[x], surface[y]);
	if (cam.surface == NULL)
		return ((t_camera){0});
	cam.depth_buffer = malloc(sizeof(float) * surface[x] * surface[y]);
	if (cam.depth_buffer == NULL)
		return ((t_camera){0});
	cam.fog = 1;
	cam.fog_color = (t_color){0};
	cam.fog_distance = 12.0f;
	cam.fov = M_PI_2;
	camera_update(&cam);
	cam.pos = (t_v3f){0.0f};
	cam.rot = (t_v2f){0.0f, 0.f};
	cam.fru_near = (t_v3f){0.f, 0.f, 0.001f};
	cam.fru_far = (t_v3f){cam.surface->size[x], cam.surface->size[y], 2000.f};
	cam.poly_clip = vector_create(sizeof(t_polygon));
	if (cam.poly_clip.data == NULL)
		return ((t_camera){0});
	cam.poly_raw = vector_create(sizeof(t_polygon));
	if (cam.poly_raw.data == NULL)
		return ((t_camera){0});
	return (cam);
}

void	camera_destroy(t_camera *const cam)
{
	vector_destroy(&cam->poly_raw);
	vector_destroy(&cam->poly_clip);
	free(cam->depth_buffer);
}

void	camera_update(t_camera *const cam)
{
	cam->fov_ratio = tanf(cam->fov / 2.f);
	cam->screen_dist = cam->surface->size[x] / 2.f / cam->fov_ratio;
	cam->fru_far[z] = cam->fog_distance;
}

void	camera_set_depth(
			t_camera *const cam,
			t_v2i const pos,
			float const depth)
{
	if (pos[x] < 0 || pos[y] < 0 || pos[x] >= cam->surface->size[x]
		|| pos[y] >= cam->surface->size[y])
		return ;
	cam->depth_buffer[pos[x] + pos[y] * cam->surface->size[x]] = depth;
}

float	camera_get_depth(t_camera *const cam, t_v2i const pos)
{
	if (pos[x] < 0 || pos[y] < 0 || pos[x] >= cam->surface->size[x]
		|| pos[y] >= cam->surface->size[y])
		return (999999.f);
	return (cam->depth_buffer[pos[x] + pos[y] * cam->surface->size[x]]);
}
