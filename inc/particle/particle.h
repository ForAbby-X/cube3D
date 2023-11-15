/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particle.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:31:26 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/13 14:49:44 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLE_H
# define PARTICLE_H

# include "game.h"

typedef struct s_particle	t_particle;

void		particles_update(t_data *const game, float const dt);

t_particle	*particle_add(
				t_data *const game,
				t_v3f const pos,
				t_v3f const dir);

t_particle	*p_fire_add(
				t_data *const game,
				t_v3f const pos,
				t_v3f const dir);

t_particle	*p_fire_explosion_add(
				t_data *const game,
				t_v3f const pos);

t_particle	*p_block_add(
				t_data *const game,
				t_v3f const pos);

t_particle	*p_spark_add(
				t_data *const game,
				t_ray const ray);

t_particle	*p_zone_add(
				t_data *const game,
				t_aabb const box,
				t_length const quantity,
				t_sprite *const spr);

t_particle	*p_small_exp(
				t_data *const game,
				t_v3f const pos,
				t_length const quantity,
				t_sprite *const spr);

void		particle_line(
				t_data *const game,
				t_v3f start,
				t_v3f end,
				t_sprite *const spr);

struct s_particle
{
	t_v3f		pos;
	t_v3f		dir;
	float		force;
	t_sprite	*spr;
	float		time_alive;
	float		death_time;
};

#endif
