/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 13:49:15 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/12 15:50:18 by alde-fre         ###   ########.fr       */
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
	float const	fsinx = sinf(cam->rot[x]);
	float const	fcosx = cosf(cam->rot[x]);
	float const	fsiny = sinf(cam->rot[y]);
	float const	fcosy = cosf(cam->rot[y]);

	plane->dist = (ft_eng_size_x(eng) / 2.0f) / tanf(cam->fov / 2.0f);
	plane->pos[x] = fcosx * fcosy;
	plane->pos[y] = fsiny;
	plane->pos[z] = fsinx * fcosy;
	plane->pos *= plane->dist;
	plane->dir_x[x] = -fsinx;
	plane->dir_x[y] = 0.0f;
	plane->dir_x[z] = fcosx;
	plane->dir_x *= (float)eng->sel_spr->size[x];
	plane->dir_y[x] = fcosx * fsiny;
	plane->dir_y[y] = -fcosy;
	plane->dir_y[z] = fsinx * fsiny;
	plane->dir_y *= (float)eng->sel_spr->size[y];
}

void	get_tex_pos(t_ray *const ray, t_v2f *const tex_pos)
{
	if (ray->side == x)
	{
		(*tex_pos)[x] = ray->end[z];
		(*tex_pos)[y] = ray->end[y];
	}
	else if (ray->side == y)
	{
		(*tex_pos)[x] = ray->end[x];
		(*tex_pos)[y] = ray->end[z];
	}
	else
	{
		(*tex_pos)[x] = ray->end[x];
		(*tex_pos)[y] = ray->end[y];
	}
	(*tex_pos)[x] -= floorf((*tex_pos)[x]);
	(*tex_pos)[y] -= floorf((*tex_pos)[y]);
	if ((ray->side == x && ray->dir[x] < 0.0f)
		|| (ray->side == z && ray->dir[z] > 0.0f))
		(*tex_pos)[x] = 1.0f - (*tex_pos)[x];
	if (ray->side == y
		&& ray->dir[y] < 0.0f)
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
	t_color			color;
	t_sprite *const	spr = map->sprites[get_real_side(ray)];

	get_tex_pos(ray, &tex_pos);
	if (map_get(map, ray->pos) != 255)
		color = ft_get_color(spr,
				(t_v2i){tex_pos[x] * spr->size[x], tex_pos[y] * spr->size[y]});
	else
		color = ft_color_f(0.f, 0.f, 0.f, tex_pos[x] + tex_pos[y]);
	if (color.a > 0 && reflections < 2)
		color = ray_reflection(map, ray, color, reflections + 1);
	if (map->fog)
		return (ft_color_inter(color, map->fog_color,
				1.0f - fmaxf(0.f, fminf(1.f,
						v3fmag(ray->end - ray->start) / map->fog_distance))));
	return (color);
}

void	ray_render(
	t_engine *const eng,
	t_map *const map,
	t_camera *const cam,
	size_t const tick)
{
	t_v2i	pix;
	t_ray	ray;
	t_v3f	dir;
	t_plane	plane;

	__setup_plane(eng, &plane, cam);
	pix[y] = 0;
	while (pix[y] < (int)eng->sel_spr->size[y])
	{
		pix[x] = tick + pix[y] & 1;
		while (pix[x] < (int)eng->sel_spr->size[x])
		{
			dir = plane.pos;
			dir += plane.dir_x * (pix[x] / (float)eng->sel_spr->size[x] - 0.5f);
			dir += plane.dir_y * (pix[y] / (float)eng->sel_spr->size[y] - 0.5f);
			ray = cast_ray(map, &cam->pos, &dir, (map->fog_distance + 1) * 2);
			ft_draw(eng, pix, ray_to_pixel(map, &ray, 0));
			pix[x] += 2;
		}
		pix[y]++;
	}
}
