/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:50:12 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 12:03:45 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "menu.h"

t_menu	menu_create(void)
{
	t_menu	menu;

	menu.data = vector_create(sizeof(t_gui));
	if (menu.data.data == NULL)
		return ((t_menu){0});
	menu.selected = -1;
	return (menu);
}

void	menu_destroy(t_menu *const menu)
{
	while (vector_size(&menu->data))
		gui_destroy(vector_pop(&menu->data));
	vector_destroy(&menu->data);
}

t_gui	*menu_add(t_menu *const menu, t_gui *const gui)
{
	return (vector_addback(&menu->data, gui));
}

void	menu_update(t_engine *const eng, t_menu *const menu)
{
	if (menu->selected >= 0 && menu->selected < vector_size(&menu->data))
		gui_update(eng, vector_get(&menu->data, menu->selected));
}

void	menu_display(t_engine *const eng, t_menu *const menu)
{
	if (menu->selected >= 0 && menu->selected < vector_size(&menu->data))
		gui_display(eng, vector_get(&menu->data, menu->selected));
}
