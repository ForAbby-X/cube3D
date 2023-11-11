/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:40:20 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/11 09:24:38 by alde-fre         ###   ########.fr       */
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

static inline t_entity	*__check_ent_hit(
						t_data *const game,
						t_v3f const pos,
						t_v3f const dir,
						float *const min_dist)
{
	t_length		index;
	t_entity		*ent;
	t_entity		*target;
	float			dist;
	int				hit;

	target = NULL;
	hit = 0;
	index = 1;
	dist = 0.0f;
	while (index < game->entities.size)
	{
		ent = &((t_entity *)game->entities.data)[index];
		if (ent->aabb.type != AABB_NONE && ent->type != ENTITY_FIREBALL
			&& ray_box_intersection(pos, dir * 999.f, ent->aabb, &dist)
			&& 999.f * dist < *min_dist)
		{
			target = ent;
			*min_dist = 999.f * dist;
		}
		++index;
	}
	return (target);
}

t_hit	retrieve_player_hit(t_data *const game)
{
	t_v3f const	dir = v3frot((t_v3f){1.f}, game->cam.rot);
	float		min_dist;
	t_entity	*target;
	t_ray		ray;
	t_hit		hit;

	ray = cast_ray(&game->map, game->cam.pos, dir, 999.f);
	min_dist = ray.dist;
	target = __check_ent_hit(game, game->cam.pos, dir, &min_dist);
	if (target == NULL || ray.dist < min_dist)
	{
		hit.type = 0;
		hit.pos = ray.end;
		hit.ray = ray;
		hit.dist = ray.dist;
	}
	else
	{
		hit.type = 1;
		hit.pos = game->cam.pos + dir * min_dist;
		hit.ent = target;
		hit.dist = min_dist;
	}
	return (hit);
}
