/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:35:11 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/10 07:08:49 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEPLAY_UTILS_H
# define GAMEPLAY_UTILS_H

# include "map.h"
# include "aabb.h"
# include "cast_ray.h"
# include "entity/entity.h"

int	ray_hit_box(
		t_map *const map,
		t_v3f const pos,
		t_v3f const dir,
		t_aabb const *const box);

int	player_use_items(t_data *const game, t_entity *const ent);

#endif
