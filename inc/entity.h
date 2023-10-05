/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 07:50:47 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/05 18:23:22 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include "game.h"

typedef struct s_entity	t_entity;
typedef size_t			t_ent_type;
typedef size_t			t_ent_id;

typedef void (*t_func_updt)(t_entity *const, t_data *const, float const);
typedef void (*t_func_disp)(t_entity *const, t_data *const);
typedef void (*t_func_dstr)(t_entity *const, t_data *const);

enum e_entity
{
ENTITY_GENERIC,
};

struct s_entity
{
	t_func_updt	update;
	t_func_disp	display;
	t_func_dstr	destroy;
	void		*data;
	t_v3f		pos;
	t_v3f		vel;
	t_v2f		rot;
	t_mesh		*mesh;
	float		alive;
	t_ent_id	uuid;
	t_ent_type	type;
};

#endif
