/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:02:01 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/19 21:20:59 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

typedef struct s_depth
{
	t_color	color;
	float	depth;
}	t_depth;

static inline void	__depth_rect(
						t_engine *const eng,
						t_camera *const cam,
						t_rect rect,
						t_depth const depth)
{
	t_v2i	p;
	t_v2i	pix;

	p[y] = ft_max(0, -rect.pos[y]);
	while (p[y] < rect.dim[x] && rect.pos[y] + p[y] < cam->surface->size[y])
	{
		p[x] = ft_max(0, -rect.pos[x]);
		while (p[x] < rect.dim[x] && rect.pos[x] + p[x] < cam->surface->size[x])
		{
			pix = rect.pos + p;
			if (depth.color.a == 0 && depth.depth < camera_get_depth(cam, pix))
			{
				ft_draw(eng, pix, depth.color);
				camera_set_depth(cam, pix, depth.depth);
			}
			p[x]++;
		}
		p[y]++;
	}
}

t_v3f	project_point(t_v3f const point, t_camera *const cam)
{
	t_v3f	proj;
	t_v3f	res;

	proj = v3froty(point - cam->pos, -cam->rot[x]);
	proj = v3frotz(proj, -cam->rot[y]);
	res = (t_v3f){
		proj[z] / proj[x] * cam->screen_dist + cam->surface->size[x] / 2.f,
		-proj[y] / proj[x] * cam->screen_dist + cam->surface->size[y] / 2.f,
		proj[x]};
	return (res);
}

void	put_spr_scale(
			t_engine *const eng,
			t_camera *const cam,
			t_dsprite const spr,
			float scale)
{
	t_v2i	pix;
	t_v2i	border;
	t_color	color;

	pix[y] = ft_max(0, -spr.pos[y]);
	border = (t_v2i){spr.spr->size[x] * scale, spr.spr->size[y] * scale};
	if (spr.pos[x] + border[x] > cam->surface->size[x])
		border[x] = cam->surface->size[x] - spr.pos[x];
	if (spr.pos[y] + border[y] > cam->surface->size[y])
		border[y] = cam->surface->size[y] - spr.pos[y];
	while (pix[y] < border[y])
	{
		pix[x] = ft_max(0, -spr.pos[x]);
		while (pix[x] < border[x])
		{
			color = ft_get_color(spr.spr,
				(t_v2i){pix[x] / scale, pix[y] / scale});
			if (color.a == 0)
				__depth_rect(eng, cam, (t_rect){spr.pos + pix,
				{ft_max(scale, 1), 0}},
					(t_depth){color, spr.depth});
			pix[x] += ft_max(scale, 1);
		}
		pix[y] += ft_max(scale, 1);
	}
}

void	put_3d_point(
			t_engine *const eng,
			t_camera *const cam,
			t_v3f const pos,
			float const ssize)
{
	t_v2i	pix;
	t_v3f	proj;
	float	size;

	proj = project_point(pos, cam);
	if (proj[z] <= cam->fru_near[z] || proj[z] >= cam->fru_far[z])
		return ;
	pix = (t_v2i){proj[x], proj[y]};
	if (proj[z] > camera_get_depth(cam, pix))
		return ;
	size = proj[z] * cam->screen_dist * ssize;
	ft_eng_sel_spr(eng, cam->surface);
	if (size > 1)
		ft_circle(eng, pix, size, (t_color){0x00EF00});
	else
		ft_draw(eng, pix, (t_color){0x00EF00});
	ft_eng_sel_spr(eng, NULL);
}

void	put_3d_spr(
			t_engine *const eng,
			t_camera *const cam,
			t_sprite *const spr,
			t_v3f const pos)
{
	t_v2i	pix;
	t_v3f	proj;
	float	size;

	proj = project_point(pos, cam);
	if (proj[z] <= cam->fru_near[z] || proj[z] >= cam->fru_far[z])
		return ;
	pix = (t_v2i){proj[x], proj[y]};
	size = (1.0f / proj[z]) * cam->screen_dist / 100.f;
	ft_eng_sel_spr(eng, cam->surface);
	put_spr_scale(eng, cam,
		(t_dsprite){pix - (t_v2i){
		spr->size[x] * size / 2.f,
		spr->size[y] * size / 2.f},
		spr, proj[z]}, size);
	ft_eng_sel_spr(eng, NULL);
}
