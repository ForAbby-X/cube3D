/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_func1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:01:22 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/14 13:56:24 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	__menu_to_fog(void *const data)
{
	t_menu_id *const	sel = data;

	*sel = 1;
}

void	__menu_to_control(void *const data)
{
	t_menu_id *const	sel = data;

	*sel = 2;
}

void	__menu_to_player(void *const data)
{
	t_menu_id *const	sel = data;

	*sel = 2;
}

void	__menu_to_settings(void *const data)
{
	t_menu_id *const	sel = data;

	*sel = 3;
}

void	__menu_launch(void *const data)
{
	int *const	sel = data;

	*sel = 0;
}
