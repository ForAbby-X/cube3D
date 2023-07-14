/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 18:18:43 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/14 10:42:22 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "vec3.h"

typedef struct s_camera	t_camera;

struct s_camera
{
	t_v3f	pos;
	t_v2f	rot;
	float	fov;
};

#endif