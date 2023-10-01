/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:17:38 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/30 12:57:50 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "agrement.h"

t_room	room_create(void)
{
	t_room	room;

	room.surface = vector_create(sizeof(t_v3i));
	if (room.surface.data == NULL)
		return ((t_room){0});
	room.entries = vector_create(sizeof(t_door));
	if (room.entries.data == NULL)
		return (vector_destroy(&room.surface), (t_room){0});
	room.linearity = 0;
	return (room);
}

t_room	room_destroy(t_room *const room)
{
	vector_destroy(&room->surface);
	vector_destroy(&room->entries);
}


