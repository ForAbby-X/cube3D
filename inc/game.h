/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:17:55 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/14 12:25:11 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "menu.h"
# include "aabb.h"

struct s_data
{
	t_map		map;
	t_map		*selected_map;
	t_sprite	*sub_screen;
	t_sprite	*minimap;
	t_camera	cam;
	t_aabb		box;
	t_menu		menu;
	size_t		tick;
	float		sensitivity;
	int			show_settings;
};

#endif
