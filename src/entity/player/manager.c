/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:05:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/10 07:03:15 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"
#include "entity/player.h"
#include "particle/particle.h"
#include "gameplay_utils.h"

static inline void	__door_loop(
						t_data *const game,
						t_entity *const self)
{
	t_v3f const	dir = v3frot((t_v3f){1.f}, game->cam.rot);
	t_entity	*ent;
	t_length	len;

	ent = game->entities.data;
	len = game->entities.size;
	while (len > 0)
	{
		if (self != ent && ent->type == ENTITY_DOOR
			&& ray_hit_box(&game->map, game->cam.pos, dir, &ent->aabb))
		{
			ent->dir[x] = 2.f;
			return ;
		}
		++ent;
		--len;
	}
}

static void	_player_update(
				t_entity *const self,
				t_data *const game,
				float const dt)
{
	t_v3f const	dir = v3frot((t_v3f){1.f}, game->cam.rot);
	t_ray		ray;

	player_control(self, game, dt);
	if (ft_mouse(game->eng, 3).pressed)
		__door_loop(game, self);
	if (self->collided)
	{
		if (ft_key(game->eng, XK_space).pressed)
			self->dir[x] = 2.5f;
		else
			self->dir[x] = 0.f;
	}
	self->dir[x] -= 9.8f * dt;
	self->vel[y] += self->dir[x] * dt;
	ray = cast_ray(&game->map, game->cam.pos, dir, 9999.f);
	if (!game->show_settings && ft_mouse(game->eng, 1).pressed
		&& game->selected_model == 3 && ray.dist < 1.f
		&& map_get(&game->map, ray.pos) == cell_wall)
	{
		if (ray.pos[y] == 3)
		{
			p_block_add(game, v3itof(ray.pos) + (t_v3f){.5f, .5f, .5f});
			map_set(&game->map, ray.pos, cell_air);
		}
		else
			p_spark_add(game, ray);
	}
	if (self->dir[z] > 0.0f)
	{
		if (self->dir[y] <= 0.0f)
		{
			p_blood_add(game, self->aabb, 5);
			self->dir[y] = 0.2f;
		}
		self->health -= dt * 5.f;
		self->dir[y] -= dt;
		self->dir[z] -= dt;
	}
	holding_update(game->eng, &game->cam, &game->holding, dt);
	game->cam.pos = self->aabb.pos + (t_v3f){0.16f, 0.7f, 0.16f};
	game->cam.pos[y] += sinf(game->holding.energy_vel * 5.f) * 0.03f;
}

static void	_player_display(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

static void	_player_destroy(t_entity *const self, t_data *const game)
{
	(void)self;
	(void)game;
}

t_entity	*e_player_add(t_data *const game, t_v3f const pos)
{
	t_entity	*ent;

	ent = entity_add(game, pos);
	if (ent == NULL)
		return (NULL);
	ent->update = &_player_update;
	ent->display = &_player_display;
	ent->destroy = &_player_destroy;
	ent->dir = (t_v3f){0};
	ent->rot = (t_v2f){0};
	ent->max_health = 100.f;
	ent->health = ent->max_health;
	ent->mesh = &game->models[5];
	ent->aabb = (t_aabb){pos - (t_v3f){0.16f, 0.f, 0.16f},
	{0.32f, 0.825f, 0.32f}, AABB_MOVABLE};
	ent->type = ENTITY_PLAYER;
	return (ent);
}
