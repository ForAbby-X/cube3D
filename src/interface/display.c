/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 12:41:43 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/25 15:14:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

static inline void	__display_check(t_engine *const eng, t_gui_obj *const obj)
{
	ft_rect(eng, g_gui_obj_offset + obj->pos, (t_v2i){18, 18},
		(t_color){0x1F1F1F});
	if (*obj->check_data)
		ft_rect(eng, g_gui_obj_offset + obj->pos + (t_v2i){4, 4},
			(t_v2i){10, 10}, (t_color){0x00b894});
	ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){20, 0}, obj->str, 2);
}

static inline void	__update_slider(t_gui_obj *const obj)
{
	t_gui_data *const	data = &obj->data;

	if (data->type == 0)
		obj->slide = (*data->f_v - data->f_v_mi)
			/ (data->f_v_ma - data->f_v_mi);
	else if (data->type == 1)
		obj->slide = (*data->i_v - data->i_v_mi)
			/ (float)(data->i_v_ma - data->i_v_mi);
	else if (data->type == 2)
		obj->slide = (*data->u_v - data->u_v_mi)
			/ (float)(data->u_v_ma - data->u_v_mi);
}

static inline void	__display_slider(t_engine *const eng, t_gui_obj *const obj)
{
	t_gui_data *const	data = &obj->data;

	if (obj->type == SLIDER)
		__update_slider(obj);
	ft_rect(eng, g_gui_obj_offset + obj->pos + (t_v2i){8, 8},
		(t_v2i){obj->size[x] - 20, 2}, (t_color){0x34495e});
	ft_rect(eng, g_gui_obj_offset + obj->pos
		+ (t_v2i){(obj->size[x] - 21) * obj->slide, 0},
		(t_v2i){18, 18},
		(t_color){0xe17055});
	if (data->type == 0)
		ft_put_nbr_f(eng, g_gui_obj_offset + obj->pos + (t_v2i){20, 0},
			*data->f_v, 2);
	else if (data->type == 1)
		ft_put_nbr(eng, g_gui_obj_offset + obj->pos + (t_v2i){20, 0},
			*data->i_v, 2);
	else if (data->type == 2)
		ft_put_nbr(eng, g_gui_obj_offset + obj->pos + (t_v2i){20, 0},
			*data->u_v, 2);
}

static inline void	__display_button(t_engine *const eng, t_gui_obj *const obj)
{
	if (obj->selected)
		ft_rect(eng, g_gui_obj_offset + obj->pos, obj->size - (t_v2i){4, 4},
		(t_color){0xbdc3c7});
	else if (obj->hovered)
		ft_rect(eng, g_gui_obj_offset + obj->pos, obj->size - (t_v2i){4, 4},
		(t_color){0x95a5a6});
	else
		ft_rect(eng, g_gui_obj_offset + obj->pos, obj->size - (t_v2i){4, 4},
		(t_color){0x7f8c8d});
	ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){2, 0}, obj->str, 2);
}

void	gui_obj_display(t_engine *const eng, t_gui_obj *const obj)
{
	if (obj->selected)
		ft_rect(eng, obj->pos, obj->size, (t_color){0x485c70});
	else if (obj->hovered)
		ft_rect(eng, obj->pos, obj->size, (t_color){0x304152});
	if (obj->type == CHECK)
		__display_check(eng, obj);
	else if (obj->type == SLIDER)
		__display_slider(eng, obj);
	else if (obj->type == TEXT && obj->str)
		ft_put_text(eng, g_gui_obj_offset + obj->pos + (t_v2i){2, 0},
			obj->str, 2);
	else if (obj->type == BUTTON)
		__display_button(eng, obj);
}
