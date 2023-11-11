/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   others.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 05:52:22 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/11 10:41:29 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle/particle.h"

t_particle	*p_blood_add(
				t_data *const game,
				t_aabb const box,
				t_length const quantity)
{
	t_length	num;
	t_particle	*particle;
	t_v3f		pos;

	num = -1;
	while (++num < quantity)
	{
		pos = box.pos + (t_v3f){
			ft_rand(-0.1f, box.dim[x] + 0.1f),
			ft_rand(0.1f, box.dim[y] + 0.1f),
			ft_rand(-0.1f, box.dim[z] + 0.1f)};
		particle = particle_add(game, pos, (t_v3f){0.f, -2.f});
		if (particle == NULL)
			continue ;
		particle->death_time = ft_rand(4.f, 6.f);
		particle->force = -1.0f;
		particle->spr = game->sprites[6];
	}
	return (particle);
}

t_particle	*p_blood_exp(
				t_data *const game,
				t_v3f const pos,
				t_length const quantity)
{
	t_length	num;
	t_particle	*particle;
	t_v3f		dir;

	num = -1;
	while (++num < quantity)
	{
		dir = v3frot((t_v3f){1.f}, (t_v2f){
			ft_rand(-M_PI, M_PI), ft_rand(-M_PI, M_PI)});
		particle = particle_add(game, pos, dir);
		if (particle == NULL)
			continue ;
		particle->death_time = ft_rand(0.1f, 0.2f);
		particle->force = ft_rand(-10.0f, -7.0f);
		particle->spr = game->sprites[6];
	}
	return (particle);
}
