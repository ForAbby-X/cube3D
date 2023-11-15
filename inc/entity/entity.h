/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 07:50:47 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 19:00:24 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include "game.h"

typedef struct s_entity	t_entity;
typedef size_t			t_ent_type;
typedef size_t			t_ent_id;

typedef void			(*t_func_updt)(
							t_entity *const s,
							t_data *const g,
							float const d);
typedef void			(*t_func_disp)(t_entity *const s, t_data *const g);
typedef void			(*t_func_dstr)(t_entity *const s, t_data *const g);

enum e_entity
{
	ENTITY_NONE,
	ENTITY_GENERIC,
	ENTITY_PLAYER,
	ENTITY_FIREBALL,
	ENTITY_MIMIC,
	ENTITY_DOOR,
	ENTITY_SPIKE,
	ENTITY_ENNEMY_SCP,
	ENTITY_ENNEMY_FISH,
	ENTITY_ROPE,
	ENTITY_HOOK,
	ENTITY_ENNEMY_CREEP,
	ENTITY_ENNEMY_NEST,
};

void	entities_update(t_data *const game, float const dt);
void	entities_collisions(t_data *const game);
void	entities_display(t_data *const game);
void	entities_destroy(t_data *const game);

int		can_see_aabb(t_data *const game, t_v3f const pos, \
		t_aabb *const box, float const dist);
int		is_point_on_screen(t_data *const game, t_v3f const pos, \
		t_aabb *const box);
int		is_entity_on_screen(t_data *const game, t_entity *const entity);
void	enemy_scp_attack(t_data *const game, t_entity *const self);

struct s_entity
{
	t_func_updt	update;
	t_func_disp	display;
	t_func_dstr	destroy;
	void		*data;
	t_v3f		dir;
	t_v3f		vel;
	t_v2f		rot;
	t_aabb		aabb;
	t_mesh		*mesh;
	float		time_alive;
	float		health;
	float		max_health;
	int			dead;
	t_ent_type	collided;
	t_ent_id	uuid;
	t_ent_type	type;
};

#endif
