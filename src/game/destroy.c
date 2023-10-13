/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 06:06:05 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/13 09:45:17 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	game_sprites_destroy(t_data *const game)
{
	size_t	index;

	index = 0;
	while (game->sprites[index])
		ft_destroy_sprite(game->eng, game->sprites[index++]);
}

void	game_models_destroy(t_data *const game)
{
	size_t	index;

	index = 0;
	while (game->models[index].spr)
		mesh_destroy(game->eng, &game->models[index++]);
}

void	game_destroy(t_data *const game)
{
	vector_destroy(&game->particles);
	vector_destroy(&game->entities);
	if (game->cam.surface)
		camera_destroy(game->eng, &game->cam);
	menu_destroy(game->eng, &game->menu);
	map_destroy(game->eng, &game->map);
	if (game->minimap)
		ft_destroy_sprite(game->eng, game->minimap);
	game_models_destroy(game);
	game_sprites_destroy(game);
}
