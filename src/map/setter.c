/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuller <vmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:03:58 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/27 17:44:56 by vmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

void	map_set(t_map *const map, t_v3i const pos, t_cell const cell)
{	
	if (pos[x] < 0 || pos[x] >= map->size[x]
		|| pos[y] < 0 || pos[y] >= map->size[y]
		|| pos[z] < 0 || pos[z] >= map->size[z])
		return ;
	map->data[pos[x]][pos[y]][pos[z]] = cell;
}

void	map_fill(
			t_map *const map,
			t_v3i pos1,
			t_v3i pos2,
			t_cell const cell)
{
	t_v3i	pos;
	t_v3i	dir;

	dir = pos2 - pos1;
	dir = (dir < (t_v3i){0, 0, 0}) + -(dir > (t_v3i){0, 0, 0});
	pos[x] = pos1[x];
	while (pos[x] != pos2[x])
	{
		pos[y] = pos1[y];
		while (pos[y] != pos2[y])
		{
			pos[z] = pos1[z];
			while (pos[z] != pos2[z])
			{
				map_set(map, pos, cell);
				pos[z] += dir[z];
			}
			pos[y] += dir[y];
		}
		pos[x] += dir[x];
	}
}
