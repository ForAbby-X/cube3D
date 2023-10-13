/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:00:31 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/13 11:17:29 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle/particle.h"

void	particles_update(t_data *const game, float const dt)
{
	t_particle	*part;
	t_length	index;

	ft_eng_sel_spr(game->eng, game->cam.surface);
	index = 0;
	while (index < game->particles.size)
	{
		part = vector_get(&game->particles, index);
		put_3d_spr(game->eng, &game->cam, part->spr, part->pos);
		part->pos += part->dir * dt;
		part->time_alive += dt;
		if (part->time_alive >= part->death_time)
			vector_erase(&game->particles, index);
		else
			index++;
	}
}
