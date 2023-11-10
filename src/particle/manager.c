/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:00:31 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/10 06:06:48 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle/particle.h"

static inline float	__clamp(float min, float max, float value)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

static inline void	__particle_colision(
						t_particle *const particle,
						t_data *const game,
						t_v3f *const vel)
{
	t_v3i	block;

	block = (t_v3i){particle->pos[x] + (*vel)[x],
		particle->pos[y], particle->pos[z]};
	if (map_get(&game->map, block) != cell_air)
		(*vel)[x] = 0.0f;
	block = (t_v3i){particle->pos[x],
		particle->pos[y] + (*vel)[y], particle->pos[z]};
	if (map_get(&game->map, block) != cell_air)
		(*vel)[y] = 0.0f;
	block = (t_v3i){particle->pos[x],
		particle->pos[y], particle->pos[z] + (*vel)[z]};
	if (map_get(&game->map, block) != cell_air)
		(*vel)[z] = 0.0f;
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
		vel = part->dir * dt;
		vel[x] = __clamp(-0.8f, 0.8f, vel[x]);
		vel[y] = __clamp(-0.8f, 0.8f, vel[y]);
		vel[z] = __clamp(-0.8f, 0.8f, vel[z]);
		__particle_colision(part, game, &vel);
		part->dir = part->dir - part->dir * part->force * dt;
		part->pos += vel;
		part->time_alive += dt;
		if (part->time_alive >= part->death_time)
			vector_erase(&game->particles, index);
		else
			index++;
	}
}
