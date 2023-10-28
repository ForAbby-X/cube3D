/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elevate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:41:55 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/28 15:10:29 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "agrement.h"

int	map_gen_elevate_rooms(t_map *const map, t_v3i const pos)
{
	t_cell	block;

	block = map_get(map, pos);
	if (block < 2048)
		return (0);
	block -= 2048;
	if (block < 6)
		map_set(map, pos + (t_v3i){0, 2}, cell_air);
	else if (block < 50)
		map_fill(map, pos + (t_v3i){0, 2}, pos + (t_v3i){0, 3, 0}, cell_air);
	else if (block < 65)
		map_fill(map, pos + (t_v3i){0, 2}, pos + (t_v3i){0, 4, 0}, cell_air);
	else
		map_fill(map, pos + (t_v3i){0, 2}, pos + (t_v3i){0, 5, 0}, cell_air);
	return (0);
}
