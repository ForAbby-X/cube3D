/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:17:55 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/24 01:36:02 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "menu.h"
# include "aabb.h"
# include "engine.h"
# include "model.h"

struct s_data
{
	t_engine	*eng;
	t_map		map;
	t_map		*selected_map;
	float		*depth_buffer;
	t_sprite	*minimap;
	t_camera	cam;
	t_aabb		box;
	t_menu		menu;
	size_t		tick;
	float		sensitivity;
	int			show_settings;
	t_sprite	*sprites[4];
	struct s_mesh		cube;
};

#endif
