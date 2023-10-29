/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:00:31 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/29 12:12:31 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle/particle.h"

static inline void	__particule_colision(
						t_particle *const particle,
						t_data *const game,
						t_v3f *const dir,
						t_v3f const vel)
{
	t_v3i	block;

	block = (t_v3i){particle->pos[x] + vel[x],
		particle->pos[y], particle->pos[z]};
	if (map_get(&game->map, block) != cell_air)
		(*dir)[x] = 0.0f;
	block = (t_v3i){particle->pos[x],
		particle->pos[y] + vel[y], particle->pos[z]};
	if (map_get(&game->map, block) != cell_air)
		(*dir)[y] = 0.0f;
	block = (t_v3i){particle->pos[x],
		particle->pos[y], particle->pos[z] + vel[z]};
	if (map_get(&game->map, block) != cell_air)
		(*dir)[z] = 0.0f;
}

void	particles_update(t_data *const game, float const dt)
{
	t_particle	*part;
	t_length	index;
	t_v3f		vel;

	ft_eng_sel_spr(game->eng, game->cam.surface);
	index = 0;
	while (index < game->particles.size)
	{
		part = vector_get(&game->particles, index);
		put_3d_spr(game->eng, &game->cam, part->spr, part->pos);
		__particule_colision(part, game, &part->dir, part->dir * dt);
		vel = part->dir * dt;
		part->dir = part->dir - part->dir * part->force * dt;
		part->pos += vel;
		part->time_alive += dt;
		if (part->time_alive >= part->death_time)
			vector_erase(&game->particles, index);
		else
			index++;
	}
}
