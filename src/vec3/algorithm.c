/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:53:58 by vmuller           #+#    #+#             */
/*   Updated: 2023/10/17 16:58:13 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"

float	v3fmag(t_v3f a)
{
	a *= a;
	return (sqrtf(a[x] + a[y] + a[z]));
}

float	v3fdot(t_v3f a, t_v3f b)
{
	return (a[x] * b[x] + a[y] * b[y] + a[z] * b[z]);
}

t_v3f	v3fcross(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		a[y] * b[z] - a[z] * b[y],
		a[z] * b[x] - a[x] * b[z],
		a[x] * b[y] - a[y] * b[x]});
}

t_v3f	v3fnorm(t_v3f a, float n)
{
	float const	mag = v3fmag(a);

	if (mag == 0.0f)
		return ((t_v3f){0});
	return (a / mag * n);
}
