/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:02:01 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/15 23:05:58 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

float	depth_get(t_data *const game, t_v2i const pos)
{
	size_t	index;

	if (pos[x] < 0 || pos[y] < 0
		|| pos[x] >= game->sub_screen->size[x]
		|| pos[y] >= game->sub_screen->size[y])
		return (-999999.f);
	index = pos[y] * game->sub_screen->size[x] + pos[x];
	return (game->depth_buffer[index]);
}

static inline void	__depth_rect(t_data *const game, t_rect rect, t_color color, float depth)
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
			if (color.a == 0 && depth < depth_get(game, pix))
			{
				ft_draw(game->eng, pix, color);
				game->depth_buffer[pix[y] * game->sub_screen->size[x] + pix[x]] = depth;
			}
			p[0]++;
		}
		p[1]++;
	}
}

void	put_spr_scale(
			t_data *const game,
			t_v2i const pos,
			t_dsprite const spr,
			float const scale)
{
	t_v2i	pix;
	t_v2i	col;
	t_color	color;

	pix[y] = 0;
	while (pix[y] < spr.spr->size[y] * scale)
	{
		pix[x] = 0;
		while (pix[x] < spr.spr->size[x] * scale)
		{
			col = (t_v2i){pix[x] / scale, pix[y] / scale};
			color = ft_get_color(spr.spr, col);
			if (game->map.fog && color.a == 0)
				__depth_rect(game, (t_rect){pos + pix,
					{ft_max(scale, 1), ft_max(scale, 1)}},
					ft_color_inter(color, game->map.fog_color, powf(1.0f
						- fmaxf(0.f, fminf(1.f, spr.depth
						/ game->map.fog_distance)), 2)),
						spr.depth);
			else
				__depth_rect(game, (t_rect){pos + pix,
					{ft_max(scale, 1), ft_max(scale, 1)}},
					color, spr.depth);
			pix[x]++;
		}
		pix[y]++;
	}
}

void	put_3d_point(t_data *const game, t_v3f const pos)
{
	t_v3f	proj;
	t_v2f	res;
	t_v2i	pix;
	float	size;
	float	value = ft_eng_size_x(game->eng) * 0.5 / tanf(game->cam.fov * 0.5);

	proj = v3froty(pos - game->cam.pos, -game->cam.rot[x]);
	proj = v3frotz(proj, -game->cam.rot[y]);
	if (proj[x] <= 0.001f)
		return ;
	res = (t_v2f){proj[z], -proj[y]} * value / proj[x];
	pix = (t_v2i){ft_eng_size_x(game->eng) / 2 + res[x], ft_eng_size_y(game->eng) / 2 + res[y]};
	size = 1.0f / proj[x];
	if (proj[x] > depth_get(game, pix))
		return ;
	if (size > 1)
		ft_circle(game->eng, pix, size, (t_color){0x00EF00});
	else
		ft_draw(game->eng, pix, (t_color){0x00EF00});
}

void	put_3d_spr(t_data *const game, t_sprite *const spr, t_v3f const pos)
{
	t_v3f	proj;
	t_v2f	res;
	t_v2i	pix;
	float	size;
	float	value = ft_eng_size_x(game->eng) * 0.5 / tanf(game->cam.fov * 0.5);

	proj = v3froty(pos - game->cam.pos, -game->cam.rot[x]);
	proj = v3frotz(proj, -game->cam.rot[y]);
	if (proj[x] <= 0.25f)
		return ;
	res = (t_v2f){proj[z], -proj[y]} * value / proj[x];
	pix = (t_v2i){ft_eng_size_x(game->eng) / 2 + res[x], ft_eng_size_y(game->eng) / 2 + res[y]};
	size = 1.0f / proj[x];
	put_spr_scale(game, pix - (t_v2i){spr->size[x] * size / 2.f, spr->size[y] * size / 2.f}, (t_dsprite){spr, proj[x]}, size);
}
