/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:24:41 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 11:12:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"

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

t_v3f	v3frot(t_v3f vec, t_v2f rot)
{
	float const	vx_sinf = sinf(rot[x]);
	float const	vx_cosf = cosf(rot[x]);
	float const	vy_sinf = sinf(rot[y]);
	float const	vy_cosf = cosf(rot[y]);
	t_v3f		res;

	res = (t_v3f){
		vec[x] * vy_cosf - vec[y] * vy_sinf,
		vec[x] * vy_sinf + vec[y] * vy_cosf,
		vec[z]};
	return ((t_v3f){
		res[x] * vx_cosf - res[z] * vx_sinf,
		res[y],
		res[x] * vx_sinf + res[z] * vx_cosf});
}

t_v2f	v3flook(t_v3f from, t_v3f to)
{
	t_v3f const	diff = to - from;
	float const	dist = v3fmag(diff);
	t_v2f		ret;

	ret[x] = atan2(diff[z], diff[x]);
	ret[y] = asinf(diff[y]) / dist;
	return (ret);
}
