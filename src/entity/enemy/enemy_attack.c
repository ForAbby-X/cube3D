/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_attack.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 04:05:25 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/13 14:50:15 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "particle/particle.h"

void	enemy_scp_attack(t_data *const game, t_entity *const self)
{
	t_entity *const	player = game->entities.data;

	(void)self;
	player->health = player->health * .8f;
	p_zone_add(game, player->aabb, 60, game->sprites[6]);
	p_small_exp(game, game->cam.pos, 60, game->sprites[6]);
}
