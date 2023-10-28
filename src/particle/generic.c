/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:52:52 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/28 13:52:25 by alde-fre         ###   ########.fr       */
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
	particle->force = 4.f;
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

	num = -1;
	rot = (t_v2f){ft_rand(-M_PI, M_PI), ft_rand(-M_PI, M_PI)};
	while (++num < 200)
	{
		particle = p_fire_add(game, pos,
				v3froty(v3frotz((t_v3f){ft_rand(3.f, 6.f)}, rot[y]), rot[x]));
		if (particle == NULL)
			break ;
		particle->death_time = ft_rand(0.6f, 0.8f);
		particle->force = ft_rand(3.0f, 5.0f);
		rot += (t_v2f){0.3f, 1.1f};
		particle = p_fire_add(game, pos,
				v3froty(v3frotz((t_v3f){ft_rand(7.f, 8.f)}, rot[y]), rot[x]));
		if (particle == NULL)
			break ;
		particle->death_time = ft_rand(0.15f, 0.35f);
		particle->force = ft_rand(0.3f, 0.8f);
		rot += (t_v2f){0.9f, 0.4f};
	}
	return (particle);
}

t_particle	*p_block_add(
				t_data *const game,
				t_v3f const pos)
{
	t_particle	*particle;
	t_length	num;

	num = -1;
	while (++num < 200)
	{
		particle = particle_add(game, pos + (t_v3f){ft_rand(-.5f, .5f),
				ft_rand(-.5f, .5f), ft_rand(-.5f, .5f)},
				(t_v3f){0.f, -2.f, 0.f});
		if (particle == NULL)
			break ;
		particle->death_time = ft_rand(0.6f, 0.8f);
		particle->force = ft_rand(-5.0f, -3.0f);
		particle->spr = game->sprites[0];
	}
	return (particle);
}
