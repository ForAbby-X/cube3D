/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 11:00:48 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/29 11:42:06 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interface.h"

t_gui	gui_create(
			t_engine *const eng,
			t_v2i const pos,
			t_v2i const size,
			char const *const title)
{
	t_gui	gui;

	gui.pos = pos;
	gui.size = size;
	gui.old_mouse_pos = (t_v2i){0, 0};
	gui.selected = 0;
	gui.title = ft_strdup(title);
	if (gui.title == NULL)
		return ((t_gui){0});
	gui.objects = vector_create(sizeof(t_gui_obj));
	if (gui.objects.data == NULL)
		return (free(gui.title), (t_gui){0});
	gui.surface = ft_sprite(eng, size[x] - 4, size[y] - 26);
	if (gui.surface == NULL)
		return (vector_destroy(&gui.objects), free(gui.title), (t_gui){0});
	gui.title_len = ft_strlen(title);
	gui.fixed = 0;
	return (gui);
}

void	gui_destroy(t_gui *const gui)
{
	vector_for_each(&gui->objects, &gui_obj_destroy);
	vector_destroy(&gui->objects);
	free(gui->title);
}

t_gui_obj	*gui_add(t_gui *const gui, t_gui_obj *const obj)
{
	int	size_x;

	obj->from = gui;
	if (obj->type == BUTTON)
		size_x = obj->str_len * 14 + 6;
	else if (obj->type == CHECK)
		size_x = obj->str_len * 14 + 4 + 20;
	else
		size_x = gui->size[x] - 4;
	obj->pos = g_gui_obj_size * (int)vector_size(&gui->objects);
	obj->size = (t_v2i){size_x, 22};
	return (vector_addback(&gui->objects, obj));
}

void	gui_update(t_engine *const eng, t_gui *const gui)
{
	t_length		index;
	t_gui_obj		*obj;

	if (eng->mouse_x >= gui->pos[x] && eng->mouse_x < gui->pos[x] + gui->size[x]
		&& eng->mouse_y >= gui->pos[y] && eng->mouse_y < gui->pos[y] + 20
		&& ft_mouse(eng, 1).pressed)
		gui->selected = 1;
	else if (ft_mouse(eng, 1).released)
		gui->selected = 0;
	if (gui->fixed == 0 && gui->selected)
		gui->pos += (t_v2i){eng->mouse_x, eng->mouse_y} - gui->old_mouse_pos;
	gui->old_mouse_pos = (t_v2i){eng->mouse_x, eng->mouse_y};
	gui->pos[x] = ft_max(0, gui->pos[x]);
	gui->pos[y] = ft_max(0, gui->pos[y]);
	gui->pos[x] = ft_min(ft_eng_size_x(eng) - gui->size[x], gui->pos[x]);
	gui->pos[y] = ft_min(ft_eng_size_y(eng) - 20, gui->pos[y]);
	index = 0;
	while (index < vector_size(&gui->objects))
	{
		obj = vector_get(&gui->objects, index);
		if (obj->visible)
			gui_obj_update(eng, obj);
		index++;
	}
}

void	gui_display(t_engine *const eng, t_gui *const gui)
{
	t_sprite *const	spr = eng->sel_spr;
	t_length		index;
	t_gui_obj		*obj;

	ft_rect(eng, gui->pos, gui->size, (t_color){0x2c3e50});
	ft_put_text(eng,
		gui->pos + (t_v2i){gui->size[x] / 2 - gui->title_len * 7, 2},
		gui->title, 2);
	ft_rect(eng, gui->pos + (t_v2i){2, 20}, (t_v2i){gui->size[x] - 4, 2},
		(t_color){0x34495e});
	ft_eng_sel_spr(eng, gui->surface);
	ft_clear(eng, (t_color){0x1e2b38});
	index = 0;
	while (index < vector_size(&gui->objects))
	{
		obj = vector_get(&gui->objects, index);
		if (obj->visible)
			gui_obj_display(eng, obj);
		index++;
	}
	ft_eng_sel_spr(eng, spr);
	ft_put_sprite(eng, gui->surface, gui->pos + g_gui_offset);
}
