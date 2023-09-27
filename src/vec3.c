/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 16:53:58 by vmuller           #+#    #+#             */
/*   Updated: 2023/09/08 10:54:07 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"


t_v3i	v3ftoi(t_v3f a)
{
	return ((t_v3i){a[x], a[y], a[z]});
}

t_v3f	v3itof(t_v3i a)
{
	return ((t_v3f){a[x], a[y], a[z]});
}

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
