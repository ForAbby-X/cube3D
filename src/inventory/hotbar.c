/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotbar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 04:54:17 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/13 05:41:49 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inventory.h"

void	hotbar_put(t_data *const game)
{
	ft_eng_sel_spr(game->eng, game->cam.surface);
	ft_put_sprite(game->eng, game->sprites[4],
		(t_v2i){0, game->eng->sel_spr->size[y] - 64});
}
