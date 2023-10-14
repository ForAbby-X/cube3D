/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:44:52 by vmuller           #+#    #+#             */
/*   Updated: 2023/10/14 23:31:42 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_H
# define VEC3_H

# include <math.h>
# include "engine.h"

typedef enum e_xyz{
	x = 0,
	y = 1,
	z = 2
}	t_xyz;

typedef float			t_v3f __attribute__((vector_size (16)));
typedef unsigned int	t_v3u __attribute__((vector_size (16)));
typedef int				t_v3i __attribute__((vector_size (16)));

t_v3i	v3ftoi(t_v3f a);
t_v3f	v3itof(t_v3i a);

float	v3fmag(t_v3f a);
float	v3fdot(t_v3f a, t_v3f b);
t_v3f	v3fcross(t_v3f a, t_v3f b);
t_v3f	v3fnorm(t_v3f a, float n);

t_v3f	v3frotx(t_v3f vec, float phi);
t_v3f	v3froty(t_v3f vec, float phi);
t_v3f	v3frotz(t_v3f vec, float phi);
t_v3f	v3frot(t_v3f vec, t_v2f rot);

#endif
