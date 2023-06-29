/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuller <vmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 13:49:15 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/27 17:44:22 by vmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "cast_ray.h"

void	map_render_iso(
			t_engine *const eng,
			t_map *const map,
			t_sprite *const spr)
{
	t_v3i	pos;

	pos[x] = 0;
	while (pos[x] < map->size[x])
	{
		pos[y] = 0;
		while (pos[y] < map->size[y])
		{
			pos[z] = 0;
			while (pos[z] < map->size[z])
			{
				if (map_get(map, pos) != 0)
					ft_put_sprite(eng, spr, (t_v2i){240, 350}
						+ (t_v2i){pos[x] * 5 - pos[z] * 5, pos[x] * 2
						- pos[y] * 5 + pos[z] * 2});
				pos[z]++;
			}
			pos[y]++;
		}
		pos[x]++;
	}
}
