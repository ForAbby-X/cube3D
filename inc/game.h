/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:17:55 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/20 05:20:03 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "engine.h"

# include "parsing.h"
# include "raycaster.h"
// #define AZERTY
# include "keys.h"
# include "menu.h"
# include "aabb.h"
# include "model.h"
# include "holding.h"
# include "inventory.h"
# include "minimap.h"

int		game_init(t_engine *const eng, t_data *const game, char **argv);

void	game_sprites_destroy(t_data *const game);
void	game_models_destroy(t_data *const game);
void	game_destroy(t_data *const game);

struct s_data
{
	t_engine	*eng;
	t_map		map;
	t_map		*selected_map;
	float		*depth_buffer;
	t_sprite	*minimap;
	t_camera	cam;
	t_menu		menu;
	size_t		tick;
	float		sensitivity;
	t_holding	holding;
	int			show_settings;
	t_sprite	*sprites[32];
	t_mesh		models[32];
	int			selected_model;
	t_vector	entities;
	t_vector	particles;
};

#endif
