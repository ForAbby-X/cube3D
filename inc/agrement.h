/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agrement.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:57:42 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/29 12:27:27 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AGREMENT_H
# define AGREMENT_H

# include "map.h"

int		map_gen_doors(t_map *const map, t_v3i const pos);
int		map_gen_rooms(t_map *const map, t_v3i const pos);

void	map_agrement(t_map *const map);

#endif
