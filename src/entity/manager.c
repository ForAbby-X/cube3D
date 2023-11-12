/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 05:50:21 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/12 05:43:23 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/entity.h"

void	entities_update(t_data *const game, float const dt)
{
	t_entity	*ent;
	t_length	index;

	ent = game->entities.data;
	index = 0;
	while (index < game->entities.size)
	{
		ent->update(ent, game, dt);
		float	mass;
		mass = ent->aabb.dim[x] * ent->aabb.dim[y] * ent->aabb.dim[z];
		ent->vel += ent->imp / mass * dt;
		ent->imp = ent->imp - ent->imp * dt * dt;
		if (ent->aabb.type == AABB_MOVABLE)
			ent->imp[y] -= 9.8f * dt;
		ent->time_alive += dt;
		++ent;
		++index;
	}
}

void	entities_display(t_data *const game)
{
	t_entity	*ent;
	t_length	index;

	ent = game->entities.data;
	index = 0;
	while (index < game->entities.size)
	{
		ent->display(ent, game);
		++ent;
		++index;
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
