/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:06:05 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/03 08:45:53 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERIC_H
# define GENERIC_H

# include "entity.h"

t_entity	entity_create(t_data *const game, t_v3f const pos);

void		entity_update(
				t_entity *const self,
				t_data *const game,
				float const dt);
void		entity_display(t_entity *const self, t_data *const game);
void		entity_destroy(t_entity *const self, t_data *const game);

#endif
