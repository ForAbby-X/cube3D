/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 03:33:54 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/13 18:08:03 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

int	ft_eng_close(t_engine *eng)
{
	mlx_loop_end(eng->mlx);
	return (0);
}

void	ft_hide_cursor(t_engine *eng)
{
	XFixesHideCursor(((t_xvar *)eng->mlx)->display,
		((t_win_list *)eng->win)->window);
}

void	ft_show_cursor(t_engine *eng)
{
	XFixesShowCursor(((t_xvar *)eng->mlx)->display,
		((t_win_list *)eng->win)->window);
}
