/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 21:56:14 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/13 11:39:55 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALL_H
# define ALL_H

# include "entity/entity.h"

t_entity	*entity_add(t_data *const game, t_v3f const pos);
t_entity	*e_fireball_add(
				t_data *const game,
				t_v3f const pos,
				t_v2f const rot);

#endif
