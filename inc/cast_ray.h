/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuller <vmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 18:18:43 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/24 14:16:55 by vmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAST_RAY_H
# define CAST_RAY_H

# include "map.h"

typedef struct s_ray	t_ray;

t_ray	cast_ray(t_map *const map, t_v3f *const pos, t_v3f *const dir);

struct s_ray
{
	t_v3i	pos;
	t_v3f	dir;
	t_v3f	start;
	t_v3f	end;
	t_v3f	delta_dist;
	t_v3f	side_dist;
	t_v3i	step;
	int		side;
	float	dist;
};

#endif