/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:31:30 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/05 15:21:30 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

t_gui_obj	gui_obj_create(
			char const *const str,
			t_gui_type const type,
			void (*on_click)(t_gui_obj *self),
			void *const value)
{
	t_gui_obj	obj;

	obj.on_click = on_click;
	obj.str = ft_strdup(str);
	if (obj.str == NULL)
		return ((t_gui_obj){0});
	obj.str_len = ft_strlen(str);
	obj.type = type;
	obj.selected = 0;
	obj.old_mouse_pos = (t_v2i){0, 0};
	obj.old_mouse_state = 0;
	obj.value = value;
	return (obj);
}

void	gui_obj_destroy(void *const obj)
{
	t_gui_obj *const	form = obj;

	free(form->str);
}

void	gui_obj_update(t_engine *const eng, t_gui_obj *const obj)
{
	t_v2i const	mouse_pos = (t_v2i){eng->mouse_x, eng->mouse_y}
		- obj->from->pos - g_gui_offset;

	obj->selected = (
			mouse_pos[x] >= obj->pos[x] && mouse_pos[y] >= obj->pos[y]
			&& mouse_pos[x] < obj->pos[x] + obj->size[x]
			&& mouse_pos[y] < obj->pos[y] + obj->size[y]);
	if (eng->mouse[1] && obj->selected)
		obj->selected = 2;
	if (obj->old_mouse_state == 0 && eng->mouse[1] && obj->on_click
		&& obj->selected)
		obj->on_click(obj);
	obj->old_mouse_pos = (t_v2i){eng->mouse_x, eng->mouse_y};
	obj->old_mouse_state = eng->mouse[1];
}

void	gui_obj_display(t_engine *const eng, t_gui_obj *const obj)
{
	if (obj->selected)
		ft_rect(eng, obj->pos,
			obj->size, (t_color [2]){{0x304152}, {0x485c70}}[obj->selected - 1]);
	if (obj->type == BUTTON)
	{
		ft_rect(eng, g_gui_obj_offset + obj->pos, (t_v2i){18, 18}, (t_color){0x1F1F1F});
		ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){20, 0}, obj->str, 2);
	}
	else if (obj->type == TEXT)
		ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){2, 0}, obj->str, 2);
}
