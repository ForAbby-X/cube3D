/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 23:28:54 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 12:07:49 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minimap.h"

static inline void	__minimap_crop(
		t_sprite *const minimap,
		t_camera *const cam)
{
	t_v2i	dim;
	t_v2i	off;
	int		dist;

	dim[y] = 0;
	while (dim[y] < minimap->size[1])
	{
		dim[0] = 0;
		while (dim[0] < minimap->size[0])
		{
			off = dim - (t_v2i){60, 60};
			dist = off[x] * off[x] + off[y] * off[y];
			if (dist >= 60 * 60)
				minimap->data[dim[x] + dim[y] * minimap->size[x]]
					= (t_color){0xFF000000};
			else if (cam->fog)
				minimap->data[dim[x] + dim[y] * minimap->size[x]]
					= ft_color_inter(minimap->data[dim[x] + dim[y]
						* minimap->size[x]], cam->fog_color,
						powf(1.0f - fmaxf(0.f, fminf(1.f, sqrtf(dist) / 10.f
									/ cam->fog_distance)), 2));
			dim[0]++;
		}
		dim[1]++;
	}
}

static inline void	minimap_display_while(
					t_engine *const eng,
					t_map *const map,
					t_camera *const cam)
{
	t_v2i		pxpos;
	t_v3i const	bcam = (t_v3i){cam->pos[x]
		+ .5f, cam->pos[y], cam->pos[z] + .5f};
	t_v2i const	off = (t_v2i){(cam->pos[x] - roundf(cam->pos[x])) * 10,
		(cam->pos[z] - roundf(cam->pos[z])) * 10};

	pxpos[y] = -7;
	while (pxpos[y] < 7)
	{
		pxpos[x] = -7;
		while (pxpos[x] < 7)
		{
			if (map_get(map, bcam + (t_v3i){pxpos[x], 0, pxpos[y]}))
				ft_rect(eng, ((t_v2i){6, 6} + pxpos) \
				* 10 - off, (t_v2i){10, 10}, (t_color){0x1f0005});
			else
				ft_put_sprite_part(eng, map->sprites[4], ((t_v2i){6, 6} \
				+ pxpos) * 10 - off, (t_rect){{0}, {10, 10}});
			pxpos[x]++;
		}
		pxpos[y]++;
	}
}

void	minimap_display(
			t_engine *const eng,
			t_map *const map,
			t_camera *const cam,
			t_sprite *const minimap)
{
	ft_eng_sel_spr(eng, minimap);
	ft_clear(eng, (t_color){0});
	minimap_display_while(eng, map, cam);
	ft_eng_sel_spr(eng, NULL);
	__minimap_crop(minimap, cam);
	ft_put_sprite_r(eng, minimap,
		(t_rect){{60, 60}, {60, 60}}, -cam->rot[x] - M_PI_2);
	ft_circle(eng, (t_v2i){6, 6} * 10, 4, (t_color){0xba1004});
}
