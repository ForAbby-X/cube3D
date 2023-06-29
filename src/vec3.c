/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:53:58 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/29 11:09:33 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"

float	v3fmag(t_v3f a)
{
	a *= a;
	return (sqrtf(a[x] + a[y] + a[z]));
}

t_v3f	v3fnorm(t_v3f a, float n)
{
	float	mag;

	mag = v3fmag(a);
	if (mag == 0.0f)
		return ((t_v3f){0});
	return (a / mag * n);
}
