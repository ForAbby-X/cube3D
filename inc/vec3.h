/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:44:52 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/29 11:08:56 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_H
# define VEC3_H

#include <math.h>

typedef enum e_xyz{
	x = 0,
	y = 1,
	z = 2
}	t_xyz;

typedef float			t_v3f __attribute__((vector_size (16)));
typedef unsigned int	t_v3u __attribute__((vector_size (16)));
typedef int				t_v3i __attribute__((vector_size (16)));

float	v3fmag(t_v3f a);
t_v3f	v3fnorm(t_v3f a, float n);

#endif