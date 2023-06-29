/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuller <vmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 17:24:53 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/27 18:45:24 by vmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static inline int	__to_map_error(
	t_engine *const eng,
	t_pars *const pars,
	t_map *const map,
	char *const str)
{
	map_destroy(eng, map);
	return (pars_error(pars, str));
}

static inline t_color	__get_color(char *str)
{
	char	*ptr;
	t_color	color;

	color.a = 20;
	ptr = ft_strchr(str, ',');
	if (ptr == NULL || ptr == str)
		return ((t_color){0xFF000000});
	color.r = ft_atoi(str);
	str = ptr + 1;
	ptr = ft_strchr(str, ',');
	if (ptr == NULL || ptr == str)
		return ((t_color){0xFF000000});
	color.g = ft_atoi(str);
	str = ptr + 1;
	ptr = ft_strchr(str, '\0');
	if (ptr == NULL || ptr == str)
		return ((t_color){0xFF000000});
	color.b = ft_atoi(str);
	return (color);
}

static inline void	__pars_to_data(t_pars *const pars, t_map *const map)
{
	t_v2i	pos;
	char	*line;

	map_fill(map, (t_v3i){0}, (t_v3i){pars->size[x], 2, pars->size[z]}, 255);
	pos[y] = 0;
	while (pos[y] < (int)ft_vector_size(pars->data))
	{
		line = ft_vector_get(pars->data, (t_length)pos[y]);
		pos[x] = 0;
		while (line[pos[x]] && line[pos[x]] != '\n')
		{
			if (line[pos[x]] == '1')
				map_set(map, (t_v3i){pos[x], 1, pos[y]}, 1);
			else if (ft_strchr("0NSEW", line[pos[x]]))
				map_set(map, (t_v3i){pos[x], 1, pos[y]}, 0);
			pos[x]++;
		}
		pos[y]++;
	}
}

static inline int	__pars_set_colors(
	t_engine *const eng,
	t_pars *const pars,
	t_map *const map)
{
	map->sky_color = __get_color(pars->elements[5]);
	if (map->sky_color.a == 0xFF)
		return (__to_map_error(eng, pars, map, "invalid sky color format"));
	map->ground_color = __get_color(pars->elements[4]);
	if (map->ground_color.a == 0xFF)
		return (__to_map_error(eng, pars, map, "invalid ground color format"));
	map->sprites[4] = ft_sprite(eng, 1, 1);
	if (map->sprites[4] == NULL)
		return (__to_map_error(eng, pars, map, "memory error on map creation"));
	map->sprites[4]->data[0] = map->sky_color;
	map->sprites[5] = ft_sprite(eng, 1, 1);
	if (map->sprites[5] == NULL)
		return (__to_map_error(eng, pars, map, "memory error on map creation"));
	map->sprites[5]->data[0] = map->ground_color;
	map->spawn[x] = pars->spawn[x] + 0.5f;
	map->spawn[y] = pars->spawn[y] + 0.5f;
	map->spawn[z] = pars->spawn[z] + 0.5f;
	return (0);
}

int	pars_to_map(
	t_engine *const eng,
	t_pars *const pars,
	t_map *const map)
{
	t_length	i;

	*map = map_create((t_v3i){pars->size[x], 15, pars->size[z]});
	if (map->data == NULL)
		return (0);
	i = 0;
	while (i < 4)
	{
		map->sprites[i] = ft_sprite(eng, 128, 128); // TO REPLACE WITH SPRITE_P
		if (map->sprites[i] == NULL)
			return (__to_map_error(eng, pars, map,
					"memory error on map creation"));
		i++;
	}
	if (__pars_set_colors(eng, pars, map))
		return (1);
	__pars_to_data(pars, map);
	return (0);
}
