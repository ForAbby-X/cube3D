/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 23:36:43 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/14 10:24:16 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H

# include "game.h"
# include "camera.h"

void	minimap_display(
			t_engine *const eng,
			t_map *const map,
			t_camera *const cam,
			t_sprite *const minimap);

#endif
