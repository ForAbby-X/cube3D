/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:17:55 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/03 08:40:41 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "menu.h"
# include "aabb.h"
# include "engine.h"
# include "model.h"

void	draw_line(
			t_engine *const eng,
			t_v2i const f,
			t_v2i const s,
			t_color const color);

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
	t_sprite	*sprites[32];
	t_mesh		models[32];
	int			selected_model;
	t_vector	entities;
};

#endif
