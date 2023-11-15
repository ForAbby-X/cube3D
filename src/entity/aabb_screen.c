/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_screen.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuller <vmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:47:15 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/11 08:28:17 by vmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/entity.h"

int	can_see_aabb(t_data *const game, t_v3f const pos, \
				t_aabb *const box, float const dist)
{
	const t_v3f	self_center = box->pos + box->dim / 2.f;
	const t_v3f	diff = v3fnorm(self_center - pos, dist);
	float		mdist;

	return (ray_box_intersection(pos, diff, *box, &mdist)
		&& cast_ray(&game->map, pos, diff, 99999.f).dist >= mdist);
}

int	is_point_on_screen(t_data *const game, t_v3f const pos, t_aabb *const box)
{
	t_v3f const	on_screen = project_point(pos, &game->cam);

	if (can_see_aabb(game, pos, box, 99999.f) == 0
		|| (on_screen[x] < 0.0f || on_screen[y] < 0.0f \
		|| on_screen[x] >= (float)game->cam.surface->size[x] \
		|| on_screen[y] >= (float)game->cam.surface->size[y] \
		|| on_screen[z] < 0.0f))
		return (0);
	return (1);
}

int	is_entity_on_screen(t_data *const game, t_entity *const entity)
{
	t_entity *const	player = game->entities.data;
	t_aabb *const	box = &player->aabb;

	if ((is_point_on_screen(game, entity->aabb.pos, box) || \
	is_point_on_screen(game, entity->aabb.pos \
	+ (t_v3f){entity->aabb.dim[x]}, box) || \
	is_point_on_screen(game, entity->aabb.pos \
	+ (t_v3f){0.f, 0.f, entity->aabb.dim[z]}, box) || \
	is_point_on_screen(game, entity->aabb.pos \
	+ (t_v3f){entity->aabb.dim[x], 0.f, entity->aabb.dim[z]}, box) || \
	is_point_on_screen(game, entity->aabb.pos \
	+ (t_v3f){0.f, entity->aabb.dim[y]}, box) || \
	is_point_on_screen(game, entity->aabb.pos \
	+ (t_v3f){entity->aabb.dim[x], entity->aabb.dim[y]}, box) || \
	is_point_on_screen(game, entity->aabb.pos + \
	(t_v3f){0.f, entity->aabb.dim[y], entity->aabb.dim[z]}, box) || \
	is_point_on_screen(game, entity->aabb.pos + \
	(t_v3f){entity->aabb.dim[x], entity->aabb.dim[y], \
		entity->aabb.dim[z]}, box)))
		return (1);
	return (0);
}
