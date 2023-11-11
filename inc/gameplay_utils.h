/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:35:11 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/11 10:42:22 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEPLAY_UTILS_H
# define GAMEPLAY_UTILS_H

# include "map.h"
# include "aabb.h"
# include "entity/entity.h"

typedef struct s_hit
{
	union
	{
		t_ray		ray;
		t_entity	*ent;
	};
	t_v3f	pos;
	float	dist;
	int		type;
}	t_hit;

int		ray_hit_box(
			t_map *const map,
			t_v3f const pos,
			t_v3f const dir,
			t_aabb const *const box);

t_hit	retrieve_player_hit(t_data *const game);
void	player_use_item(t_data *const game, t_hit const hit);

#endif
