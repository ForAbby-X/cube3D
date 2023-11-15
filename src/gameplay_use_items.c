/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_use_items.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:40:20 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 19:22:14 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gameplay_utils.h"
#include "particle/particle.h"
#include "entity/all.h"

static float const	g_damage[] = {
	6.f, 0.f, 28.f, 18.f, 6.f, 999.f, 0.f,
	0.f, 34.f, 0.f, 0.f, 0.f, 0.f, 16.f
};

static float const	g_reach[] = {
	1.f, 1.f, 1.5f, 1.f, .75f, 999.f, 1.f,
	1.f, 2.f, 1.f, 1.f, 1.f, 1.f, 1.f
};

void	effect_explosion(t_data *const game, t_v3f const pos)
{
	t_entity	*ent;
	t_length	index;

	index = 0;
	p_fire_explosion_add(game, pos);
	while (index < game->entities.size)
	{
		ent = &((t_entity *)game->entities.data)[index];
		if (ent->aabb.type != AABB_NONE && ent->type != ENTITY_HOOK
			&& ent->type != ENTITY_FIREBALL && ent->type != ENTITY_ENNEMY_SCP
			&& ent->type != ENTITY_MIMIC && ent->type != ENTITY_ROPE
			&& ent->type != ENTITY_DOOR
			&& can_see_aabb(game, pos, &ent->aabb, 1.2f))
		{
			p_small_exp(game, ent->aabb.pos + ent->aabb.dim / 2.f,
				30, game->sprites[6]);
			p_zone_add(game, ent->aabb, 30, game->sprites[6]);
			ent->health -= 30.f
				/ (v3fmag(ent->aabb.pos + ent->aabb.dim / 2.f - pos) * 2.f);
			if (ent->type != ENTITY_PLAYER)
				ent->dead = ent->health <= 0;
		}
		++index;
	}
}

static inline void	__attack2(
						t_data *const game,
						t_hit const hit,
						t_entity *const ent,
						float const	dam)
{
	if (ent->type == ENTITY_ENNEMY_NEST)
	{
		p_zone_add(game, ent->aabb, 30, game->sprites[9]);
		p_small_exp(game, hit.pos, 20, game->sprites[9]);
		ent->health -= dam;
		if (ent->health <= 0)
			e_enemy_fish_add(game,
				ent->aabb.pos + (t_v3f){ent->aabb.dim[x] / 2.f, 0.f,
				ent->aabb.dim[z] / 2.f}, (t_v2f){ft_rand(-M_PI, M_PI), 0.f});
		ent->dead = ent->health <= 0;
	}
	if (ent->type == ENTITY_ENNEMY_SCP
		&& (game->selected_model == 3 || game->selected_model == 13))
	{
		if (game->selected_model == 3)
		{
			game->selected_model = 13;
			ent->health -= ent->max_health * 0.5f;
		}
		else
			ent->health -= ent->max_health * 0.25f;
		p_zone_add(game, ent->aabb, 20, game->sprites[0]);
		p_small_exp(game, hit.pos, 10, game->sprites[0]);
		ent->dead = ent->health <= 0;
	}
}

static inline void	__attack(
						t_data *const game,
						t_hit const hit,
						t_entity *const ent)
{
	float const	dam = g_damage[game->selected_model] + ft_rand(-2.5f, 2.5f);

	if (game->selected_model == 5)
	{
		p_zone_add(game, ent->aabb, 30, game->sprites[6]);
		p_small_exp(game, hit.pos, 20, game->sprites[6]);
		ent->health -= dam;
		ent->dead = ent->health <= 0;
	}
	if (ent->type == ENTITY_ENNEMY_CREEP || ent->type == ENTITY_ENNEMY_FISH)
	{
		p_zone_add(game, ent->aabb, 30, game->sprites[6]);
		p_small_exp(game, hit.pos, 20, game->sprites[6]);
		ent->health -= dam;
		ent->dead = ent->health <= 0;
	}
	__attack2(game, hit, ent, dam);
}

static int const	g_items[] = {4, 4, 4, 3, 3, 3, 2, 2, 2, 8, 8, 0};

static inline void	__use(t_data *const game, t_entity *const ent)
{
	int	temp;

	if (ent->type == ENTITY_DOOR && ent->dir[x] < 1.f)
	{
		game->holding.click_energy = 0.25f;
		ent->dir[x] = 2.f;
	}
	if (ent->type == ENTITY_MIMIC)
	{
		game->holding.click_energy = 0.25f;
		if ((((size_t)ent->data) & 0x10) == 0)
			ent->data = (void *)((long)16 + g_items[rand() % 12]);
		else
		{
			temp = game->selected_model;
			game->selected_model = ((long)ent->data) & 0xF;
			ent->data = (void *)((long)16 + temp);
		}
	}
}

void	player_use_item(t_data *const game, t_hit const hit)
{
	float	hit_dist;

	hit_dist = g_reach[game->selected_model];
	if (ft_mouse(game->eng, 1).pressed)
		game->holding.click_energy = 0.5f;
	if (ft_mouse(game->eng, 1).pressed && game->selected_model == 0)
		e_fireball_add(game, game->cam.pos, game->cam.rot);
	else if (ft_mouse(game->eng, 1).pressed && hit.dist <= hit_dist)
	{
		if (hit.type == 0)
		{
			p_spark_add(game, hit.ray);
			if (hit.ray.pos[y] == 3 && game->selected_model == 3)
			{
				p_block_add(game, v3itof(hit.ray.pos) + (t_v3f){.5f, .5f, .5f});
				map_set(&game->map, hit.ray.pos, cell_air);
				game->selected_model = 13;
			}
		}
		else
			__attack(game, hit, hit.ent);
	}
	else if (ft_mouse(game->eng, 3).pressed && hit.type == 1 && hit.dist <= 1.f)
		__use(game, hit.ent);
}
