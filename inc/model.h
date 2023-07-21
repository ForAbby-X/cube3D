/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:02:13 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/21 11:50:01 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_H
# define MODEL_H

# include "vec3.h"
# include "game.h"

typedef struct s_dsprite	t_dsprite;

struct s_dsprite
{
	t_v2i		pos;
	t_sprite	*spr;
	float		depth;
};

t_v3f	project_point(t_v3f const point, t_camera *const cam);

void	put_spr_scale(
			t_engine *const eng,
			t_camera *const cam,
			t_dsprite const spr,
			float const scale);
void	put_3d_point(
			t_engine *const eng,
			t_camera *const cam,
			t_v3f const pos);
void	put_3d_spr(
			t_engine *const eng,
			t_camera *const cam,
			t_sprite *const spr,
			t_v3f const pos);

#endif
