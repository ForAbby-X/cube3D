/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:31:30 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/07 18:24:07 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

static inline void	__swap_int(t_gui_obj *self)
{
	int *const	value = self->value;

	*value = !*value;
}

t_gui_obj	gui_obj_create(
	char const *const str,
	t_gui_type const type,
	void (*on_click)(t_gui_obj *self),
	void *const value)
{
	t_gui_obj	obj;

	if (on_click == NULL && obj.type == BUTTON)
		obj.on_click = &__swap_int;
	else
		obj.on_click = on_click;
	obj.str = ft_strdup(str);
	if (obj.str == NULL)
		return ((t_gui_obj){0});
	obj.str_len = ft_strlen(str);
	obj.type = type;
	obj.selected = 0;
	obj.hovered = 0;
	obj.slide = 0.5f;
	obj.old_mouse_pos = (t_v2i){0, 0};
	obj.value = value;
	return (obj);
}

void	gui_obj_destroy(void *const obj)
{
	t_gui_obj *const	form = obj;

	free(form->str);
}

static inline int	_is_inside(
		t_v2i const pos,
		t_v2i const pos1,
		t_v2i const dim)
{
	return (pos[x] >= pos1[x] && pos[y] >= pos1[y]
		&& pos[x] < pos1[x] + dim[x]
		&& pos[y] < pos1[y] + dim[y]);
}

void	gui_obj_update(t_engine *const eng, t_gui_obj *const obj)
{
	t_v2i const	mouse_pos = (t_v2i){eng->mouse_x, eng->mouse_y}
		- obj->from->pos - g_gui_offset;

	if (!_is_inside((t_v2i){eng->mouse_x, eng->mouse_y},
		obj->from->pos, obj->from->size) || obj->value == NULL)
	{
		obj->hovered = 0;
		obj->selected = 0;
		return ;
	}
	obj->hovered = _is_inside(mouse_pos, obj->pos, obj->size);
	if (obj->hovered && ft_mouse(eng, 1).pressed)
		obj->selected = 1;
	if (ft_mouse(eng, 1).hold && obj->selected && obj->type == SLIDER)
	{
		obj->slide = (eng->mouse_x - obj->from->pos[x] - obj->pos[x] - 10)
			/ (float)(obj->size[x]);
		obj->slide = fmaxf(0.0f, fminf(1.0f, obj->slide));
		obj->on_click(obj);
	}
	if (ft_mouse(eng, 1).released)
	{
		if (obj->on_click && (obj->selected && obj->hovered)
			&& obj->type != SLIDER)
			obj->on_click(obj);
		obj->selected = 0;
	}
	obj->old_mouse_pos = (t_v2i){eng->mouse_x, eng->mouse_y};
}

void	gui_obj_display(t_engine *const eng, t_gui_obj *const obj)
{
	if (obj->selected)
		ft_rect(eng, obj->pos, obj->size, (t_color){0x485c70});
	else if (obj->hovered)
		ft_rect(eng, obj->pos, obj->size, (t_color){0x304152});
	if (obj->type == BUTTON)
	{
		ft_rect(eng, g_gui_obj_offset + obj->pos, (t_v2i){18, 18},
			(t_color){0x1F1F1F});
		if (*(int *)obj->value)
			ft_rect(eng, g_gui_obj_offset + obj->pos + (t_v2i){4, 4},
				(t_v2i){10, 10}, (t_color){0x00b894});
		ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){20, 0},
			obj->str, 2);
	}
	else if (obj->type == SLIDER)
	{
		ft_rect(eng, g_gui_obj_offset + obj->pos + (t_v2i){8, 8},
			(t_v2i){obj->size[x] - 16, 2}, (t_color){0x34495e});
		ft_rect(eng, g_gui_obj_offset + obj->pos
			+ (t_v2i){(obj->size[x]) * obj->slide, 0},
			(t_v2i){18, 18},
			(t_color){0xe17055});
		ft_put_nbr(eng, g_gui_obj_offset + obj->pos + (t_v2i){20, 0},
			obj->slide * 100.f, 2);
	}
	else if (obj->type == TEXT)
		ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){2, 0},
			obj->str, 2);
}
