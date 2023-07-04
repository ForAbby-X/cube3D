/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:31:30 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/04 22:50:38 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

t_gui_obj	gui_obj_create(
			char const *const str,
			t_gui_type const type,
			int (*on_click)(t_gui_obj *self),
			int *const value)
{
	t_gui_obj	obj;

	obj.on_click = on_click;
	obj.str = ft_strdup(str);
	if (obj.str == NULL)
		return ((t_gui_obj){0});
	obj.str_len = ft_strlen(str);
	obj.type = type;
	obj.value = value;
	return (obj);
}

void	gui_obj_destroy(void *const obj)
{
	t_gui_obj *const	form = obj;

	free(form->str);
}

void	gui_obj_display(t_engine *const eng, t_gui_obj *const obj)
{
	if (obj->type == BUTTON)
	{
		ft_rect(eng, g_gui_obj_offset + obj->pos, (t_v2i){18, 18}, (t_color){0x1F1F1F});
		ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){20, 0}, obj->str, 2);
	}
	if (obj->type == TEXT)
	{
		ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){2, 0}, obj->str, 2);
		ft_rect(eng, g_gui_obj_offset + obj->pos + (t_v2i){0, 18}, (t_v2i){obj->str_len * 14 + 14, 2},
			(t_color){0x34495e});
		ft_rect(eng, g_gui_obj_offset + obj->pos, (t_v2i){2, 18},
			(t_color){0x34495e});
	}
}
