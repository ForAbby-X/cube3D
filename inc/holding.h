/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holding.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:24:57 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/13 06:05:05 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOLDING_H
# define HOLDING_H

# include "model.h"

typedef struct s_holding
{
	t_v3f	last_pos;
	float	energy_vel;
	float	click_energy;
	float	time_from_start;
}	t_holding;

void	holding_update(
			t_camera *const cam,
			t_holding *const hold,
			float const dt);
void	holding_display(
			t_engine *const eng,
			t_camera *const cam,
			t_mesh *const models,
			t_holding *const hold);

#endif
