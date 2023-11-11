/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_use_items.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:40:20 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/11 10:41:56 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gameplay_utils.h"
#include "particle/particle.h"

static inline void	__attack(
						t_data *const game,
						t_hit const hit,
						t_entity *const ent)
{
	p_blood_add(game, ent->aabb, 10);
	p_blood_exp(game, hit.pos, 10);
}

static inline void	__use(t_data *const game, t_entity *const ent)
{
	(void)game;
	if (ent->type == ENTITY_DOOR)
		ent->dir[x] = 2.f;
}

void	player_use_item(t_data *const game, t_hit const hit)
{
	float	hit_dist;

	hit_dist = 1.f;
	if (game->selected_model == 2)
		hit_dist = 1.5f;
	else if (game->selected_model == 4)
		hit_dist = 0.75f;
	if (ft_mouse(game->eng, 1).pressed && hit.dist <= hit_dist)
	{
		if (hit.type == 0)
		{
			p_spark_add(game, hit.ray);
			if (hit.ray.pos[y] == 3 && game->selected_model == 3)
			{
				p_block_add(game, v3itof(hit.ray.pos) + (t_v3f){.5f, .5f, .5f});
				map_set(&game->map, hit.ray.pos, cell_air);
			}
		}
		else
			__attack(game, hit, hit.ent);
	}
	if (ft_mouse(game->eng, 3).pressed && hit.type == 1 && hit.dist <= 1.f)
		__use(game, hit.ent);
}