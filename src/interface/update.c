/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 12:50:24 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/13 18:37:26 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

static inline int	_is_inside(
		t_v2i const pos,
		t_v2i const pos1,
		t_v2i const dim)
{
	return (pos[x] >= pos1[x] && pos[y] >= pos1[y]
		&& pos[x] < pos1[x] + dim[x]
		&& pos[y] < pos1[y] + dim[y]);
}

static inline void	__change_slider(t_engine *const eng, t_gui_obj *const obj)
{
	t_gui_data *const	data = &obj->data;

	obj->slide = (eng->mouse_x - obj->from->pos[x] - obj->pos[x] - 10)
		/ (float)(obj->size[x] - 20);
	obj->slide = fmaxf(0.0f, fminf(1.0f, obj->slide));
	if (data->type == 0)
		*data->f_v = obj->slide * (data->f_v_ma - data->f_v_mi) + data->f_v_mi;
	else if (data->type == 1)
		*data->i_v = obj->slide * (data->i_v_ma - data->i_v_mi) + data->i_v_mi;
	else if (data->type == 2)
		*data->u_v = obj->slide * (data->u_v_ma - data->u_v_mi) + data->u_v_mi;
}

void	gui_obj_update(t_engine *const eng, t_gui_obj *const obj)
{
	t_v2i const	mouse_pos = (t_v2i){eng->mouse_x, eng->mouse_y}
		- obj->from->pos - g_gui_offset;

	if (obj->type == TEXT)
	{
		obj->selected = 0;
		return ;
	}
	if (!_is_inside((t_v2i){eng->mouse_x, eng->mouse_y},
		obj->from->pos, obj->from->size))
		obj->hovered = 0;
	obj->hovered = _is_inside(mouse_pos, obj->pos, obj->size);
	if (obj->hovered && ft_mouse(eng, 1).pressed)
		obj->selected = 1;
	if (ft_mouse(eng, 1).hold && obj->selected && obj->type == SLIDER)
		__change_slider(eng, obj);
	if (ft_mouse(eng, 1).released)
	{
		if (obj->selected && obj->hovered && obj->type == CHECK)
			*obj->check_data = !*obj->check_data;
		else if (obj->selected && obj->hovered && obj->type == BUTTON)
			obj->on_click(obj->on_click_data);
		obj->selected = 0;
	}
	obj->old_mouse_pos = (t_v2i){eng->mouse_x, eng->mouse_y};
}
