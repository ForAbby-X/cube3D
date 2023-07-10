/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_tint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 17:46:23 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/10 17:55:39 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_sprite	*load_tint_sprite(
	t_engine *eng,
	char *const path,
	t_color const tint)
{
	t_sprite *const	spr = ft_sprite_p(eng, path);
	int				index;
	t_color			color;
	float			grey;

	if (spr == NULL)
		return (NULL);
	index = 0;
	while (index < spr->size[x] * spr->size[y])
	{
		color = spr->data[index];
		grey = ft_color_med(color) / 255.f;
		spr->data[index] = ft_color(0.f,
				tint.r * grey, tint.g * grey, tint.b * grey);
		index++;
	}
	return (spr);
}
