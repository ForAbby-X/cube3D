/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 05:50:21 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/10 03:18:47 by alde-fre         ###   ########.fr       */
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
		put_3d_point(game->eng, &game->cam, ent->aabb.pos, 0.05f);
		put_3d_point(game->eng, &game->cam, ent->aabb.pos + ent->aabb.dim, 0.05f);
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
