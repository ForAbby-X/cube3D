/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 08:45:45 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/14 18:15:44 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "title.h"

static void	_hook_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	t_entity *const	player = game->entities.data;

	(void)dt;
	if (ft_mouse(game->eng, 3).pressed && \
		ray_box_intersection(game->cam.pos, v3frot((t_v3f){1.0f}, \
		game->cam.rot), self->aabb, &(float){0.0f}) && game->state == 1)
	{
		player->aabb.type = AABB_IMMOVABLE;
		game->cam.fog_distance = 4.f;
		game->death_pos = self->aabb.pos + self->aabb.dim * .5f;
		game->state = 2;
		title_put(&game->title, g_titles[2], 10.0f);
	}
}

static void	_hook_display(t_entity *const self, t_data *const game)
{
	t_entity *const	player = game->entities.data;
	t_transform		trans;

	trans.rotation = self->rot;
	trans.resize = (t_v3f){.8f, .8f, .8f};
	trans.translation = self->aabb.pos + (t_v3f){0.3f, 0.55f, 0.3f};
	mesh_put(game->eng, &game->cam, trans, &game->models[11]);
	if (game->state == 2 && player->health > 0.0f)
		mesh_put(game->eng, &game->cam, (t_transform) \
			{{self->rot[x] + sinf(game->holding.time_from_start) * 0.03f,
			cosf(game->holding.time_from_start * 0.842f) * 0.03f}, \
			{.6f, .6f, .6f}, cast_ray(&game->map, game->map.spawn, \
			(t_v3f){0.f, 1.0f}, 99999.0f).end - (t_v3f){0.f, .465f}},
			&game->models[12]);
}

static void	_hook_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

t_entity	*e_hook_add(t_data *const game, t_v3f const pos)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_hook_update;
	ent->display = &_hook_display;
	ent->destroy = &_hook_destroy;
	ent->dir = (t_v3f){0};
	ent->rot = (t_v2f){ft_rand(-M_PI, M_PI), 0.0f};
	ent->mesh = &game->models[12];
	ent->aabb.dim = (t_v3f){0.6f, 0.6f, 0.6f};
	ent->aabb.pos = pos - (t_v3f){0.3f, 0.62f, 0.3f};
	ent->aabb.type = AABB_IMMOVABLE;
	ent->type = ENTITY_HOOK;
	return (ent);
}
