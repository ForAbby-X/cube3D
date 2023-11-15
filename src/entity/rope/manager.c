/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 08:20:51 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/14 18:12:31 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "title.h"

static void	_rope_update(
			t_entity *const self,
			t_data *const game,
			float const dt)
{
	(void)dt;
	if (ft_mouse(game->eng, 3).pressed && \
		ray_box_intersection(game->cam.pos, v3frot((t_v3f){1.0f}, \
		game->cam.rot), self->aabb, &(float){0.0f}))
	{
		self->dead = 1;
		game->state = 1;
		game->cam.fog_color = (t_color){0xFF8A0F0F};
		title_put(&game->title, g_titles[1], 2.5f);
	}
}

static void	_rope_display(t_entity *const self, t_data *const game)
{
	t_transform	trans;

	trans.rotation = self->rot;
	trans.resize = (t_v3f){1., 1.f, 1.f};
	trans.translation = self->aabb.pos
		+ (t_v3f){self->aabb.dim[x], 0.f, self->aabb.dim[z]} * 0.5f;
	mesh_put(game->eng, &game->cam, trans, &game->models[10]);
}

static inline void	__fix_rope(
						t_data *const game,
						t_v3f const pos,
						t_entity *const rope)
{
	t_v3i const	cpos = v3ftoi(pos);

	rope->aabb.dim = (t_v3f){0.3f, 0.7f, 0.3f};
	if (map_get(&game->map, cpos + (t_v3i){1, 0, 0}) == cell_wall)
	{
		rope->aabb.pos = v3itof(cpos) + (t_v3f){.85f, 0.f, 0.35f};
		rope->rot = (t_v2f){0, 0.f};
	}
	else if (map_get(&game->map, cpos + (t_v3i){0, 0, 1}) == cell_wall)
	{
		rope->aabb.pos = v3itof(cpos) + (t_v3f){0.35f, 0.f, 0.85f};
		rope->rot = (t_v2f){M_PI_2, 0.f};
	}
	else if (map_get(&game->map, cpos + (t_v3i){-1, 0, 0}) == cell_wall)
	{
		rope->aabb.pos = v3itof(cpos) + (t_v3f){-.15f, 0.f, 0.35f};
		rope->rot = (t_v2f){M_PI, 0.f};
	}
	else
	{
		rope->aabb.pos = v3itof(cpos) + (t_v3f){0.35f, 0.f, -0.15f};
		rope->rot = (t_v2f){-M_PI_2, 0.f};
	}
}

t_entity	*e_rope_add(t_data *const game, t_v3f const pos)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_rope_update;
	ent->display = &_rope_display;
	ent->mesh = &game->models[10];
	__fix_rope(game, pos, ent);
	ent->aabb.type = AABB_IMMOVABLE;
	ent->type = ENTITY_ROPE;
	return (ent);
}
