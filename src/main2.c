/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 17:45:10 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 19:21:17 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "entity/entity.h"
#include "entity/all.h"
#include "particle/particle.h"
#include "aabb.h"
#include "gameplay_utils.h"

int	loop_la_vrai(t_engine *eng, t_data *game, float const dt)
{
	t_v3f	dir;

	if (game->state == 2)
		ft_put_text(eng, (t_v2i){3, eng->sel_spr->size[y] - 28},
			"[ESC] to free yourself...", 2);
	if (game->state == 1)
	{
		game->cam.fog_distance = fmaxf(game->cam.fog_distance - dt, 6.0f);
		if (game->hard_time >= 0.25f)
		{
			game->hard_time = 0.0f;
			dir = v3frot((t_v3f){ft_rand(1.f, 4.f)},
					(t_v2f){ft_rand(-M_PI, M_PI)});
			e_enemy_fish_add(game, (t_v3f){game->cam.pos[x], 2.7f,
				game->cam.pos[z]} + dir, (t_v2f){ft_rand(-M_PI, M_PI)});
		}
		game->hard_time += dt;
	}
	game->holding.time_from_start += dt;
	return (1);
}
