/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 14:49:50 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/28 15:45:57 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "generation.h"

static inline void	__foreach_block(t_data *const game, t_v3i const pos)
{
	t_cell const	block = map_get(&game->map, pos);
	t_v3f const		center = v3itof(pos) + (t_v3f){.5f, 2.f, .5f};

	if (block == cell_door_z)
		e_door_add(game, center, (t_v2f){0.f, 0.f});
	else if (block == cell_door_x)
		e_door_add(game, center, (t_v2f){M_PI_2, 0.f});
	else if (block >= cell_zone)
		generate_room(game, pos);
}

void	map_generate(t_data *const game)
{
	t_v3i	pos;

	pos[y] = 1;
	pos[x] = 0;
	while (pos[x] < game->map.size[x])
	{
		pos[z] = 0;
		while (pos[z] < game->map.size[z])
		{
			__foreach_block(game, pos);
			++pos[z];
		}
		++pos[x];
	}
}
