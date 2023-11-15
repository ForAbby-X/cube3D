/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:23:30 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/14 16:48:37 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "title.h"

void	title_put(t_title *const title, char const *const str, float const time)
{
	title->time = time;
	title->str = (char *)str;
}

void	title_update(t_engine *const eng, t_title *const title, float const dt)
{
	t_v2i const	center = {eng->sel_spr->size[0] / 2, \
	eng->sel_spr->size[1] / 2 + eng->sel_spr->size[1] / 3};

	if (title->time > 0.0f)
	{
		ft_put_text(eng, (t_v2i){center[0] - \
		ft_strlen(title->str) / 2 * 7, center[1]}, title->str, 1);
		title->time = fmaxf(title->time - dt, 0);
	}
}
