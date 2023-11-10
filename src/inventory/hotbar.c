/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotbar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 04:54:17 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/10 06:46:27 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inventory.h"
#include "entity/entity.h"

void	hotbar_put(t_data *const game)
{
	t_entity *const	player = game->entities.data;
	float const		ratio = player->health / player->max_health;

	ft_eng_sel_spr(game->eng, game->cam.surface);
	ft_put_sprite_part(game->eng, game->sprites[4],
		(t_v2i){0, game->eng->sel_spr->size[y] - 64 * ratio},
		(t_rect){{0, 64 * (1.f - ratio)}, {64, 64 * ratio}});
	ft_put_sprite_part(game->eng, game->sprites[8],
		(t_v2i){0, game->eng->sel_spr->size[y] - 64},
		(t_rect){{0, 0}, {64, 64 * (1.f - ratio)}});
}
