/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 23:28:54 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/14 10:40:26 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minimap.h"

// t_v2i	pos;
// 	t_v2i	rpos;
// 	t_v2i	wpos;

// 	pos[1] = -2;
// 	while (++pos[1] <= data->cam.dim[1] / 32 + 1)
// 	{
// 		rpos[1] = (pos[1]
// 				- (data->cam.pos[1] / 32.f - data->cam.pos[1] / 32)) * 32;
// 		pos[0] = -2;
// 		while (++pos[0] <= data->cam.dim[0] / 32 + 1)
// 		{
// 			rpos[0] = (pos[0]
// 					- (data->cam.pos[0] / 32.f - data->cam.pos[0] / 32)) * 32;
// 			wpos = pos + data->cam.pos / 32;
// 			ft_put_sprite(data->eng, data->spr[2 + 31
// 				* (ft_get_map(data->map, wpos) == 'E')], rpos);
// 			if (ft_get_map(data->map, wpos) == '1')
// 				ft_put_sprite(data->eng, data->spr[3], rpos);
// 			else
// 				ft_put_sprite_part(data->eng, data->map->background, rpos,
// 					(t_rect){wpos * 32, (t_v2i){32, 32}});
// 		}
// 	}

static inline void	__minimap_crop(
		t_sprite *const minimap,
		t_map *const map)
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
			else if (map->fog)
				minimap->data[dim[x] + dim[y] * minimap->size[x]]
				= ft_color_inter(minimap->data[dim[x] + dim[y] * minimap->size[x]],
					map->fog_color,
					powf(1.0f - fmaxf(0.f, fminf(1.f, sqrtf(dist) / 10.f
					/ map->fog_distance)), 2));	
			dim[0]++;
		}
		dim[1]++;
	}
}

void	minimap_display(
			t_engine *const eng,
			t_map *const map,
			t_camera *const cam,
			t_sprite *const minimap)
{
	t_v2i	pxpos;
	t_v3i	bcam;
	t_v2i	off;

	ft_eng_sel_spr(eng, minimap);
	ft_clear(eng, (t_color){0});
	pxpos[y] = -7;
	while (pxpos[y] < 7)
	{
		pxpos[x] = -7;
		while (pxpos[x] < 7)
		{
			bcam = (t_v3i){cam->pos[x] + .5f, cam->pos[y], cam->pos[z] + .5f};
			off = (t_v2i){(cam->pos[x] - roundf(cam->pos[x])) * 10, (cam->pos[z] - roundf(cam->pos[z])) * 10};
			if (map_get(map, bcam + (t_v3i){pxpos[x], 0, pxpos[y]}))
				ft_rect(eng, ((t_v2i){6, 6} + pxpos) * 10 - off, (t_v2i){10, 10}, (t_color){0x1f0005});
				// ft_rect(eng, ((t_v2i){6, 6} + pxpos) * 10 - off, (t_v2i){10, 10}, (t_color){0x32a852});
			else
				ft_put_sprite_part(eng, map->sprites[4], ((t_v2i){6, 6} + pxpos) * 10 - off, (t_rect){{0}, {10, 10}});
			pxpos[x]++;
		}
		pxpos[y]++;
	}
	ft_eng_sel_spr(eng, NULL);
	__minimap_crop(minimap, map);
	ft_put_sprite_r(eng, minimap, (t_rect){{60, 60}, {60, 60}}, -cam->rot[x] - M_PI_2);
	ft_circle(eng, (t_v2i){6, 6} * 10, 4, (t_color){0xba1004});
}