/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 04:32:32 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/14 23:53:31 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

static inline t_v3f	__v3froty(t_v3f vec, t_v2f sc)
{
	return ((t_v3f){
		vec[x] * sc[1] - vec[z] * sc[0],
		vec[y],
		vec[x] * sc[0] + vec[z] * sc[1]});
}

static inline t_v3f	__v3frotz(t_v3f vec, t_v2f sc)
{
	return ((t_v3f){
		vec[x] * sc[1] - vec[y] * sc[0],
		vec[x] * sc[0] + vec[y] * sc[1],
		vec[z]});
}

void	vert_transform(
			t_vert *const vert,
			t_transform const tran,
			t_v3f const v_sc)
{
	vert[0].point *= tran.resize;
	vert[1].point *= tran.resize;
	vert[2].point *= tran.resize;
	vert[0].point = __v3frotz(vert[0].point, *((t_v2f *)&v_sc + 1));
	vert[0].point = __v3froty(vert[0].point, *(t_v2f *)&v_sc);
	vert[1].point = __v3frotz(vert[1].point, *((t_v2f *)&v_sc + 1));
	vert[1].point = __v3froty(vert[1].point, *(t_v2f *)&v_sc);
	vert[2].point = __v3frotz(vert[2].point, *((t_v2f *)&v_sc + 1));
	vert[2].point = __v3froty(vert[2].point, *(t_v2f *)&v_sc);
	vert[0].point += tran.translation;
	vert[1].point += tran.translation;
	vert[2].point += tran.translation;
}
