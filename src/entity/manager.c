/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 05:50:21 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/21 01:24:23 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/entity.h"

void	entities_update(t_data *const game, float const dt)
{
	t_entity	*ent;
	t_length	len;

	ent = game->entities.data;
	len = game->entities.size;
	while (len > 0)
	{
		ent->update(ent, game, dt);
		ent->time_alive += dt;
		ent++;
		len--;
	}
}

void	entities_collisions(t_data *const game)
{
	t_entity	*ent;
	t_length	len;

	ent = game->entities.data;
	len = game->entities.size;
	while (len > 0)
	{
		// COLLISIONS
		
		ent++;
		len--;
	}
}

void	entities_display(t_data *const game)
{
	t_entity	*ent;
	t_length	len;

	ent = game->entities.data;
	len = game->entities.size;
	while (len > 0)
	{
		ent->display(ent, game);
		ent++;
		len--;
	}
}

void	entities_destroy(t_data *const game)
{
	t_entity	*ent;
	t_length	index;

	index = 0;
	while (index < game->entities.size)
	{
		ent = vector_get(&game->entities, index);
		if (ent->dead)
		{
			ent->destroy(ent, game);
			vector_erase(&game->entities, index);
		}
		else
			index++;
	}
}
