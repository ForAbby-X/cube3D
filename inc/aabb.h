/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 21:01:42 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/08 21:03:26 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AABB_H
# define AABB_H

# include "vec3.h"

int	is_aabb_in_aab(
		t_v3f const pos1, t_v3f const dim1,
		t_v3f const pos2, t_v3f const dim2);

#endif
