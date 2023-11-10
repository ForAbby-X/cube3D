/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:40:20 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/10 07:02:46 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gameplay_utils.h"
#include "particle/particle.h"

int	ray_hit_box(
		t_map *const map,
		t_v3f const pos,
		t_v3f const dir,
		t_aabb const *const box)
{
	float		cdist;

	return (ray_box_intersection(pos, dir, *box, &cdist)
		&& cast_ray(map, pos, dir, 99999.f).dist > cdist);
}

int	player_use_items(t_data *const game, t_entity *const ent)
{
	t_v3f const	dir = v3frot((t_v3f){1.f}, game->cam.rot);

	if (!ft_mouse(game->eng, 1).pressed)
		return (0);
	if (game->selected_model == 2 && ray_hit_box(&game->map, game->cam.pos,
			dir * 1.5f, &ent->aabb))
		p_blood_add(game, ent->aabb, 10);
	return (0);
}
