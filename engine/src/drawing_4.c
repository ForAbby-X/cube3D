/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 15:43:01 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/03 17:27:39 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

static inline int	ft_abs(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

enum {x, y};

void	draw_line(
		t_engine *const eng,
		t_v2i const f,
		t_v2i const s,
		t_color const color)
{
	t_v2i const	delta = {ft_abs(s[x] - f[x]), ft_abs(s[y] - f[y])};
	t_v2i const	sign = -(f < s) + (f >= s);
	int			error[2];
	t_v2i		cur;
	size_t		len;

	error[0] = delta[x] - delta[y];
	len = ft_max(delta[x], delta[y]);
	cur = f;
	while (len)
	{
		ft_draw(eng, cur, color);
		error[1] = error[0] * 2;
		if (error[1] > -delta[y])
		{
			error[0] -= delta[y];
			cur[x] += sign[x];
		}
		if (error[1] < delta[x])
		{
			error[0] += delta[x];
			cur[y] += sign[y];
		}
		len--;
	}
}
