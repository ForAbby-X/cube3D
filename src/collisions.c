/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 20:47:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/08 21:03:55 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"

int	is_aabb_in_aab(
	t_v3f const pos1, t_v3f const dim1,
	t_v3f const pos2, t_v3f const dim2)
{
	return ((pos1[x] + dim1[x] >= pos2[x] || pos1[x] <= pos2[x] + dim2[x])
		&& (pos1[y] + dim1[y] >= pos2[y] || pos1[y] <= pos2[y] + dim2[y]));
}


