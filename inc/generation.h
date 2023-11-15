/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 14:50:35 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/13 18:35:55 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERATION_H
# define GENERATION_H

# include "game.h"
# include "entity/entity.h"
# include "entity/all.h"
# include "particle/particle.h"

void	map_generate(t_data *const game);

void	generate_room(t_data *const game, t_v3i const pos);

int		generate_rope(t_data *const game);

#endif
