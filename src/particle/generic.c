/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:52:52 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/17 17:54:26 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle/particle.h"

t_particle	*particle_add(
				t_data *const game,
				t_v3f const pos,
				t_v3f const dir)
{
	t_particle	*particle;

	particle = vector_addback(&game->particles,
			&(t_particle){0});
	if (particle == NULL)
		return (NULL);
	particle->pos = pos;
	particle->dir = dir;
	particle->death_time = 2.f;
	return (particle);
}

t_particle	*p_fire_add(
				t_data *const game,
				t_v3f const pos,
				t_v3f const dir)
{
	t_particle *const	particle = particle_add(game, pos, dir);

	if (particle == NULL)
		return (NULL);
	particle->spr = game->sprites[2];
	particle->death_time = ft_rand(1.6f, 2.f);
	return (particle);
}

t_particle	*p_fire_explosion_add(
				t_data *const game,
				t_v3f const pos)
{
	t_particle	*particle;
	t_length	num;
	t_v2f		rot;

	num = 0;
	rot = (t_v2f){0.f, 0.f};
	while (num < 300)
	{
		particle = p_fire_add(game, pos,
				v3froty(v3frotz((t_v3f){ft_rand(3.f, 6.f)}, rot[y]), rot[x]));
		if (particle == NULL)
			break ;
		particle->death_time = ft_rand(0.3f, 0.9f);
		rot += (t_v2f){0.9f, 0.4f};
		++num;
	}
	return (particle);
}
