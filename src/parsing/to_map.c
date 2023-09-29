/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 17:24:53 by vmuller           #+#    #+#             */
/*   Updated: 2023/09/29 12:01:03 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "agrement.h"

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
	t_color	color;
	int		err;
	int		i;

	color.d = 20;
	i = 0;
	while (i < 3)
	{
		if (!ft_isdigit(*str))
			return ((t_color){0xFF000000});
		color.d = (color.d << 8) | ft_ato_u8(str, &err);
		while (ft_isdigit(*str))
			str++;
		if ((i == 2 && *str != '\0') || err)
			return ((t_color){0xFF000000});
		if (i < 2 && *str != ',')
			return ((t_color){0xFF000000});
		str++;
		i++;
	}
	return (color);
}

static inline void	__pars_to_data(t_pars *const pars, t_map *const map)
{
	t_v2i	pos;
	char	*line;

	map_fill(map, (t_v3i){0},
		(t_v3i){pars->size[x], 2, pars->size[z]}, cell_wall);
	map_fill(map, (t_v3i){0, 1, 0},
		(t_v3i){pars->size[x], 1, pars->size[z]}, cell_void);
	pos[y] = 0;
	while (pos[y] < (int)vector_size(&pars->data))
	{
		line = *((char **)vector_get(&pars->data, (t_length)pos[y]));
		pos[x] = 0;
		while (line[pos[x]] && line[pos[x]] != '\n')
		{
			if (line[pos[x]] == '1')
				map_set(map, (t_v3i){pos[x], 1, pos[y]}, cell_wall);
			else if (ft_strchr("0NSEW", line[pos[x]]))
				map_set(map, (t_v3i){pos[x], 1, pos[y]}, cell_air);
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
	if (map->sky_color.d == 0xFF000000)
		return (__to_map_error(eng, pars, map, "invalid sky color format"));
	map->ground_color = __get_color(pars->elements[4]);
	if (map->ground_color.d == 0xFF000000)
		return (__to_map_error(eng, pars, map, "invalid ground color format"));
	map->sprites[4]
		= load_tint_sprite(eng, "assets/ceilling.xpm", map->sky_color);
	if (map->sprites[4] == NULL)
		return (__to_map_error(eng, pars, map, "memory error on map creation"));
	map->sprites[5]
		= load_tint_sprite(eng, "assets/floor.xpm", map->ground_color);
	if (map->sprites[5] == NULL)
		return (__to_map_error(eng, pars, map, "memory error on map creation"));
	map->spawn[x] = pars->spawn[x] + 0.5f;
	map->spawn[y] = pars->spawn[y];
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
		map->sprites[i] = ft_sprite_p(eng, pars->elements[i]);
		if (map->sprites[i] == NULL)
			return (__to_map_error(eng, pars, map,
					"memory error on map creation"));
		i++;
	}
	if (__pars_set_colors(eng, pars, map))
		return (1);
	__pars_to_data(pars, map);
	map_agrement(map);
	return (0);
}
