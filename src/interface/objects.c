/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:31:30 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/04 16:05:06 by alde-fre         ###   ########.fr       */
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
	(void)eng;
	(void)obj;
}
