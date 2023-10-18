/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:09:11 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/18 12:28:55 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

void	shader_apply_depth(t_camera *const cam)
{
	size_t	index;
	float	value;

	index = 0;
	while (index < (size_t)cam->surface->size[x] * cam->surface->size[y])
	{
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
