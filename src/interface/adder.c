/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 15:23:21 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/08 17:29:37 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

t_gui_obj	*gui_add_text(t_gui *const gui, char const *const str)
{
	t_gui_obj	obj;

	obj = gui_obj_create(str, TEXT, NULL, (t_gui_data){0});
	return (gui_add(gui, &obj));
}

t_gui_obj	*gui_add_check(
		t_gui *const gui,
		char const *const str,
		int *const on_click_data)
{
	t_gui_obj	obj;

	obj = gui_obj_create(str, CHECK, NULL, (t_gui_data){0});
	obj.check_data = on_click_data;
	return (gui_add(gui, &obj));
}

t_gui_obj	*gui_add_slider(t_gui *const gui, t_gui_data data)
{
	t_gui_obj	obj;

	obj = gui_obj_create(NULL, SLIDER, NULL, data);
	return (gui_add(gui, &obj));
}

t_gui_obj	*gui_add_button(
	t_gui *const gui,
	char const *const str,
	t_obj_meth on_click,
	void *const on_click_data)
{
	t_gui_obj	obj;

	obj = gui_obj_create(str, BUTTON, on_click, (t_gui_data){0});
	obj.on_click_data = on_click_data;
	return (gui_add(gui, &obj));
}
