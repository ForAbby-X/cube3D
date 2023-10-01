/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 04:32:32 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/01 04:43:14 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

void	vert_rotate(t_vert *const vert, t_v2f const rot)
{
	vert[0].point = v3frotz(v3froty(vert[0].point, rot[x]), rot[y]);
	vert[1].point = v3frotz(v3froty(vert[1].point, rot[x]), rot[y]);
	vert[2].point = v3frotz(v3froty(vert[2].point, rot[x]), rot[y]);
}
