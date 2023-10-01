/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agrement.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:57:42 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/01 04:30:28 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AGREMENT_H
# define AGREMENT_H

# include "map.h"

typedef struct s_door	t_door;
typedef struct s_room	t_room;

//TODO HANDLE ROOM SAVING AND ADDING RANDOM ITEMS TO THEM

int		map_gen_doors(t_map *const map, t_v3i const pos);
int		map_gen_rooms(t_map *const map, t_v3i const pos);
int		map_gen_merge_rooms(t_map *const map, t_v3i const pos);
int		map_gen_elevate_rooms(t_map *const map, t_v3i const pos);

void	map_agrement(t_map *const map);

struct s_door
{
	t_v3i		pos;
	t_v3i		dir;
	t_length	length;
};

struct s_room
{
	t_vector	surface;
	t_vector	entries;
	int			linearity;
};

#endif
