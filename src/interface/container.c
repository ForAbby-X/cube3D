/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 11:00:48 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/04 16:15:51 by alde-fre         ###   ########.fr       */
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
	gui.anchor = LEFT;
	gui.title = ft_strdup(title);
	if (gui.title == NULL)
		return ((t_gui){0});
	gui.objects = vector_create(sizeof(t_gui_obj));
	if (gui.objects.data == NULL)
		return (free(gui.title), (t_gui){0});
	gui.surface = ft_sprite(eng, size[x] - 4, size[y] - 24);
	if (gui.surface == NULL)
		return (vector_destroy(&gui.objects), free(gui.title), (t_gui){0});
	gui.title_len = ft_strlen(title);
	return (gui);
}

void	gui_destroy(t_engine *const eng, t_gui *const gui)
{
	vector_for_each(&gui->objects, &gui_obj_destroy);
	ft_destroy_sprite(eng, gui->surface);
	vector_destroy(&gui->objects);
	free(gui->title);
}

t_gui_obj	*gui_add(t_gui *const gui, t_gui_obj *const obj)
{
	obj->from = gui;
	return (vector_addback(&gui->objects, obj));
}

void	gui_display(t_engine *const eng, t_gui *const gui)
{
	t_sprite	*spr;

	ft_rect(eng, gui->pos, gui->size, (t_color){0x2c3e50});
	ft_put_text(eng,
		(t_v2i){gui->size[x] / 2 - gui->title_len * 7, 2}, gui->title, 2);
	ft_rect(eng, (t_v2i){2, 20}, (t_v2i){gui->size[x] - 4, 2},
		(t_color){0x34495e});
	spr = eng->sel_spr;
	ft_eng_sel_spr(eng, gui->surface);
	ft_clear(eng, (t_color){0xFF000000});
	gui_obj_display(eng, vector_get(&gui->objects, 0), (t_v2i){0, 0});
	ft_eng_sel_spr(eng, spr);
	ft_put_sprite(eng, gui->surface, gui->pos + (t_v2i){2, 22});
}
