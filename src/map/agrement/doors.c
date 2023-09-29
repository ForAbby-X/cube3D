/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:55:30 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/29 21:36:54 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "agrement.h"

static inline void	__simple_doors(t_map *const map, t_v3i const pos)
{
	if (map_get(map, pos + (t_v3i){1, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){-1, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){0, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, -1}) == 0)
		map_set(map, pos, cell_door);
	if (map_get(map, pos + (t_v3i){0, 0, 1}) == 1
		&& map_get(map, pos + (t_v3i){0, 0, -1}) == 1
		&& map_get(map, pos + (t_v3i){1, 0, 0}) == 0
		&& map_get(map, pos + (t_v3i){-1, 0, 0}) == 0)
		map_set(map, pos, cell_door);
}

static inline void	__double_doors(t_map *const map, t_v3i const pos)
{
	if (map_get(map, pos + (t_v3i){-1, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){1, 0, 0}) == 0
		&& map_get(map, pos + (t_v3i){2, 0, 0}) == 1
		&& map_get(map, pos + (t_v3i){1, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){1, 0, -1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, -1}) == 0)
		map_fill(map, pos, pos + (t_v3i){1, 0, 0}, cell_door);
	if (map_get(map, pos + (t_v3i){0, 0, -1}) == 1
		&& map_get(map, pos + (t_v3i){0, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){0, 0, 2}) == 1
		&& map_get(map, pos + (t_v3i){1, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){-1, 0, 1}) == 0
		&& map_get(map, pos + (t_v3i){1, 0, 0}) == 0
		&& map_get(map, pos + (t_v3i){-1, 0, 0}) == 0)
		map_fill(map, pos, pos + (t_v3i){0, 0, 1}, cell_door);
}

int	map_gen_doors(t_map *const map, t_v3i const pos)
{
	if (map_get(map, pos) != 0)
		return (0);
	__simple_doors(map, pos);
	__double_doors(map, pos);
	return (0);
}
