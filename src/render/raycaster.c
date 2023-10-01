/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 13:49:15 by vmuller           #+#    #+#             */
/*   Updated: 2023/09/30 23:44:43 by alde-fre         ###   ########.fr       */
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

	plane->pos[x] = fcosx * fcosy;
	plane->pos[y] = fsiny;
	plane->pos[z] = fsinx * fcosy;
	plane->pos *= cam->screen_dist;
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
	t_sprite		*spr;
	t_cell			block;

	if (ray->dist >= 99999.f)
		return ((t_color){0});
	spr = map->sprites[get_real_side(ray)];
	block = map_get(map, ray->pos);
	get_tex_pos(ray, &tex_pos);
	if (block == cell_wall)
		color = ft_get_color(spr,
				(t_v2i){tex_pos[x] * spr->size[x], tex_pos[y] * spr->size[y]});
	else if (block == cell_door)
		color = ft_color_f(0.f, fabsf(tex_pos[x] - .5f), 0.f, fabsf(tex_pos[y] - .5f));
	else if (block >= 2048)
		color = ft_color(0, (block - 2048), 126, 255 - (block - 2048) * 10);
	if (color.a > 0 && reflections < 2)
		color = ray_reflection(map, ray, color, reflections + 1);
	return (color);
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
	t_color	col;

	ft_eng_sel_spr(eng, cam->surface);
	__setup_plane(eng, &plane, cam);
	pix[y] = 0;
	while (pix[y] < cam->surface->size[y])
	{
		pix[x] = 0;
		while (pix[x] < cam->surface->size[x])
		{
			dir = plane.pos;
			dir += plane.dir_x * (pix[x] / (float)cam->surface->size[x] - 0.5f);
			dir += plane.dir_y * (pix[y] / (float)cam->surface->size[y] - 0.5f);
			ray = cast_ray(map, &cam->pos, &dir, cam->fog_distance
				/ cam->screen_dist);
			camera_set_depth(cam, pix, ray.dist * cam->screen_dist);
			col = ray_to_pixel(map, &ray, 0);
			ft_draw(eng, pix, col);
			pix[x]++;
		}
		pix[y]++;
	}
	ft_eng_sel_spr(eng, NULL);
}
