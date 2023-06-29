/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 13:49:15 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/29 12:13:07 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"

typedef struct s_plane
{
	t_v3f	pos;
	t_v3f	dir_x;
	t_v3f	dir_y;
	float	dist;
}	t_plane;

static inline void	__setup_plane(
	t_engine *const eng,
	t_plane *const plane,
	t_camera *const cam)
{
	float const	screen_ratio = (float)ft_eng_size_y(eng)
		/ (float)ft_eng_size_x(eng);

	plane->dist = ft_eng_size_x(eng) / 2.0f / tanf(cam->fov / 2.0f);
	plane->pos[x] = cosf(cam->rot[x]) * cosf(cam->rot[y]);
	plane->pos[y] = sinf(cam->rot[y]);
	plane->pos[z] = sinf(cam->rot[x]) * cosf(cam->rot[y]);
	plane->pos *= plane->dist;
	plane->dir_x[x] = cosf(cam->rot[x] + M_PI_2);
	plane->dir_x[y] = 0.0f;
	plane->dir_x[z] = sinf(cam->rot[x] + M_PI_2);
	plane->dir_x *= (float)ft_eng_size_x(eng) / 2.0f;
	plane->dir_y[x] = cosf(cam->rot[x]) * cosf(cam->rot[y] - M_PI_2);
	plane->dir_y[y] = sinf(cam->rot[y] - M_PI_2);
	plane->dir_y[z] = sinf(cam->rot[x]) * cosf(cam->rot[y] - M_PI_2);
	plane->dir_y *= (float)ft_eng_size_y(eng) / 2.0f;
	plane->dir_y *= screen_ratio;
}

void	get_tex_pos(t_ray *const ray, t_v2f *const tex_pos)
{
	if (ray->side == x)
	{
		(*tex_pos)[x] = ray->start[z] + ray->dist * ray->dir[z];
		(*tex_pos)[y] = ray->start[y] + ray->dist * ray->dir[y];
	}
	else if (ray->side == y)
	{
		(*tex_pos)[x] = ray->start[x] + ray->dist * ray->dir[x];
		(*tex_pos)[y] = ray->start[z] + ray->dist * ray->dir[z];
	}
	else
	{
		(*tex_pos)[x] = ray->start[x] + ray->dist * ray->dir[x];
		(*tex_pos)[y] = ray->start[y] + ray->dist * ray->dir[y];
	}
	(*tex_pos)[x] -= floorf((*tex_pos)[x]);
	(*tex_pos)[y] -= floorf((*tex_pos)[y]);
	if ((ray->side == x && ray->dir[x] < 0.0f)
		|| (ray->side == z && ray->dir[z] > 0.0f))
		(*tex_pos)[x] = 1.0f - (*tex_pos)[x];
	if (ray->side == y && ray->dir[y] < 0.0f)
		(*tex_pos)[y] = 1.0f - (*tex_pos)[y];
	(*tex_pos)[y] = 1.0f - (*tex_pos)[y];
}

t_cell_side	get_real_side(t_ray *const ray)
{
	if (ray->side == x)
		return (ray->dir[x] > 0.0f);
	else if (ray->side == z)
		return (west + (ray->dir[z] > 0.0f));
	else
		return (up + (ray->dir[y] > 0.0f));
}

t_color	ray_to_pixel(
	t_map *const map,
	t_ray *const ray,
	long reflections)
{
	t_v2f			tex_pos;
	t_v2i			pix_pos;
	t_color			color;
	t_sprite *const	spr = map->sprites[get_real_side(ray)];
	float const		dist = 255.0f
		- fminf(255.0f, fmax(0.0f, ray->dist * 1000.0f));

	get_tex_pos(ray, &tex_pos);
	pix_pos = (t_v2i){tex_pos[x] * spr->size[x], tex_pos[y] * spr->size[y]};
	color = ft_get_color(spr, pix_pos);
	if (color.g > 0 && reflections < 0)
		color = ray_reflection(map, ray, color, reflections + 1);
	return (ft_color_inter(color, (t_color){0}, dist / 255.0f));
}

void	ray_render(
	t_engine *const eng,
	t_map *const map,
	t_camera *const cam)
{
	t_v2i	pix;
	t_ray	ray;
	t_v3f	dir;
	t_plane	plane;

	__setup_plane(eng, &plane, cam);
	pix[y] = 0;
	while (pix[y] < (int)ft_eng_size_y(eng))
	{
		pix[x] = 0;
		while (pix[x] < (int)ft_eng_size_x(eng))
		{
			dir = plane.pos;
			t_v2f pp = {(pix[x] / (float)ft_eng_size_x(eng) - 0.5f), (pix[y] / (float)ft_eng_size_y(eng) - 0.5f)};
			//pp = pp + pp + pp;
			dir += plane.dir_x * pp[x];
			dir += plane.dir_y * pp[y];
			ray = cast_ray(map, &cam->pos, &dir);
			ft_draw(eng, pix, ray_to_pixel(map, &ray, 0));
			pix[x]++;
		}
		pix[y]++;
	}
}
