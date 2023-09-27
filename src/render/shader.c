/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:09:11 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/23 07:38:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

// ft_draw(eng, pix, ft_color_inter(col, cam->fog_color,
// 						powf(1.0f - fmaxf(0.f, fminf(1.f,
// 									camera_get_depth(cam, pix)
// 									/ cam->fog_distance)), 2)));

void	shader_apply_depth(t_camera *const cam)
{
	size_t	index;
	float	value;
	// float	fog;
	// float	light;
	t_v2f	pix;

	index = 0;
	while (index < (size_t)cam->surface->size[x] * cam->surface->size[y])
	{
		pix = (t_v2f){
			(index % cam->surface->size[x]) - (cam->surface->size[x] / 2.f),
			(index / cam->surface->size[x]) - (cam->surface->size[y] / 2.f)};

		// light = 1.0f - fmaxf(0.f, fminf(1.f, fog / 12.f));
		// fog = fminf(cam->depth_buffer[index], light);

		value = 1.0f - fmaxf(0.f, fminf(1.f,
					cam->depth_buffer[index] / cam->fog_distance));
		value = value * value;

		cam->surface->data[index].r *= value;
		cam->surface->data[index].g *= value;
		cam->surface->data[index].b *= value;
		cam->surface->data[index].r += cam->fog_color.r * (1.f - value);
		cam->surface->data[index].g += cam->fog_color.g * (1.f - value);
		cam->surface->data[index].b += cam->fog_color.b * (1.f - value);

		index++;
	}
}
