/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:53:58 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/18 16:38:26 by alde-fre         ###   ########.fr       */
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

t_v3f	v3frotx(t_v3f vec, float phi)
{
	float const	v_sinf = sinf(phi);
	float const	v_cosf = cosf(phi);

	return ((t_v3f){
		vec[x],
		vec[y] * v_cosf - vec[z] * v_sinf,
		vec[y] * v_sinf + vec[z] * v_cosf});
}

t_v3f	v3froty(t_v3f vec, float phi)
{
	float const	v_sinf = sinf(phi);
	float const	v_cosf = cosf(phi);

	return ((t_v3f){
		vec[x] * v_cosf - vec[z] * v_sinf,
		vec[y],
		vec[x] * v_sinf + vec[z] * v_cosf});
}

t_v3f	v3frotz(t_v3f vec, float phi)
{
	float const	v_sinf = sinf(phi);
	float const	v_cosf = cosf(phi);

	return ((t_v3f){
		vec[x] * v_cosf - vec[y] * v_sinf,
		vec[x] * v_sinf + vec[y] * v_cosf,
		vec[z]});
}
