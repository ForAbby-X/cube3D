/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:02:13 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/15 22:19:56 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_H
# define MODEL_H

# include "vec3.h"
# include "game.h"

typedef struct s_dsprite	t_dsprite;

struct s_dsprite
{
	t_sprite	*spr;
	float		depth;
};

float	depth_get(t_data *const game, t_v2i const pos);
void	put_spr_scale(
			t_data *const game,
			t_v2i const pos,
			t_dsprite const spr,
			float const scale);
void	put_3d_point(t_data *const game, t_v3f const pos);
void	put_3d_spr(t_data *const game, t_sprite *const spr, t_v3f const pos);

#endif