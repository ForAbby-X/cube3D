/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:02:01 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/24 01:40:02 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

static inline void	__depth_rect(
						t_engine *const eng,
						t_camera *const cam,
						t_rect const rect,
						t_color const color,
						float const depth)
{
	t_v2i	p;
	t_v2i	pix;

	p[1] = 0;
	while (p[1] < rect.dim[1])
	{
		p[0] = 0;
		while (p[0] < rect.dim[0])
		{
			pix = rect.pos + p;
			if (color.a == 0 && depth < camera_get_depth(cam, pix))
			{
				ft_draw(eng, pix, color);
				camera_set_depth(cam, pix, depth);
			}
			p[0]++;
		}
		p[1]++;
	}
}

t_v3f	project_point(t_v3f const point, t_camera *const cam)
{
	t_v3f	proj;
	t_v3f	res;

	proj = v3froty(point - cam->pos, -cam->rot[x]);
	proj = v3frotz(proj, -cam->rot[y]);
	if (proj[x] <= 0.01f)
		return ((t_v3f){0.0f});
	res = (t_v3f){
		proj[z] * cam->screen_dist / proj[x] + cam->surface->size[x] / 2.f,
		-proj[y] * cam->screen_dist / proj[x] + cam->surface->size[y] / 2.f,
		proj[x]};
	return (res);
}

void	put_spr_scale(
			t_engine *const eng,
			t_camera *const cam,
			t_dsprite const spr,
			float const scale)
{
	t_v2i	pix;
	t_v2i	col;
	t_color	color;

	pix[y] = ft_max(-spr.pos[y], 0);
	while (pix[y] < spr.spr->size[y] * scale && pix[y] < cam->surface->size[y])
	{
		pix[x] = ft_max(-spr.pos[x], 0);
		while (pix[x] < spr.spr->size[x] * scale && pix[x] < cam->surface->size[x])
		{
			col = (t_v2i){pix[x] / scale, pix[y] / scale};
			color = ft_get_color(spr.spr, col);
			if (cam->fog && color.a == 0)
				__depth_rect(eng, cam, (t_rect){spr.pos + pix,
					{ft_max(scale, 1), ft_max(scale, 1)}},
					ft_color_inter(color, cam->fog_color, powf(1.0f
						- fmaxf(0.f, fminf(1.f, spr.depth
						/ cam->fog_distance)), 2)),
						spr.depth);
			else
				__depth_rect(eng, cam, (t_rect){spr.pos + pix,
					{ft_max(scale, 1), ft_max(scale, 1)}},
					color, spr.depth);
			pix[x]++;
		}
		pix[y]++;
	}
}

void	put_3d_point(
			t_engine *const eng,
			t_camera *const cam,
			t_v3f const pos)
{
	t_v2i	pix;
	t_v3f	proj;
	float	size;

	proj = project_point(pos, cam);
	if (proj[z] <= 0.01f)
		return ;
	pix = (t_v2i){proj[x], proj[y]};
	if (proj[z] > camera_get_depth(cam, pix))
		return ;
	size = 1.0f / proj[z] / cam->fov_ratio;
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
	if (proj[z] <= 0.25f)
		return ;
	pix = (t_v2i){proj[x], proj[y]};
	if (proj[z] > camera_get_depth(cam, pix))
		return ;
	size = 1.0f / proj[z] / cam->fov_ratio;
	ft_eng_sel_spr(eng, cam->surface);
	put_spr_scale(eng, cam,
		(t_dsprite){pix - (t_v2i){
		spr->size[x] * size / 2.f,
		spr->size[y] * size / 2.f},
		spr, proj[z]}, size);
	ft_eng_sel_spr(eng, NULL);
}
