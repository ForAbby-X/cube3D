/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 13:08:16 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/29 16:17:56 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "agrement.h"

void	map_agrement(t_map *const map)
{
	map_iterate(map, map_gen_doors);
	map_iterate(map, map_gen_rooms);
	map_iterate(map, map_gen_merge_rooms);
	map_iterate(map, map_gen_elevate_rooms);
}
