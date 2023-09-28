/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:43:15 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/28 11:52:44 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	map_iterate(
		t_map *const map,
		int (*func)(t_map *const map, t_v3i const pos))
{
	t_v3i		pos;
	int			error;

	pos[y] = 1;
	pos[z] = 0;
	while (pos[z] < map->size[z])
	{
		pos[x] = 0;
		while (pos[x] < map->size[x])
		{
			error = func(map, pos);
			if (error)
				return (error);
			pos[x]++;
		}
		pos[z]++;
	}
	return (0);
}