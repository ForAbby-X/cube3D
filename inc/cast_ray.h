/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:08:35 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/08 16:30:46 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAST_RAY_H
# define CAST_RAY_H

# include "map.h"

typedef struct s_ray	t_ray;

t_ray	cast_ray(
			t_map *const map,
			t_v3f *const pos,
			t_v3f *const dir,
			float const max_dist);

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
