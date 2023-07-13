/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:31:30 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/13 15:31:55 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

t_gui_obj	gui_obj_create(
	char const *const str,
	t_gui_type const type,
	t_obj_meth on_click,
	t_gui_data data)
{
	t_gui_obj	obj;

	obj.on_click = on_click;
	obj.str = NULL;
	if (str)
	{
		obj.str = ft_strdup(str);
		if (obj.str == NULL)
			return ((t_gui_obj){0});
		obj.str_len = ft_strlen(str);
	}
	obj.type = type;
	obj.selected = 0;
	obj.hovered = 0;
	obj.old_mouse_pos = (t_v2i){0, 0};
	obj.data = data;
	obj.visible = 1;
	return (obj);
}

void	gui_obj_destroy(void *const obj)
{
	t_gui_obj *const	form = obj;

	free(form->str);
}
