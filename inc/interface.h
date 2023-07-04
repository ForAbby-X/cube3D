/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 15:23:29 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/04 16:09:53 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

# include "engine.h"
# include "vec3.h"

typedef enum e_gui_anchor		t_gui_anchor;
typedef struct s_gui_container	t_gui;
typedef enum e_gui_type			t_gui_type;
typedef struct s_gui_obj		t_gui_obj;

enum e_gui_anchor
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	CENTER
};

struct s_gui_container
{
	t_v2i			pos;
	t_v2i			size;
	t_gui_anchor	anchor;
	t_vector		objects;
	char			*title;
	size_t			title_len;
	t_sprite		*surface;
};

enum e_gui_type
{
	NONE,
	IMAGE,
	BUTTON,
	SLIDER,
	TEXT
};

struct s_gui_obj
{
	int			*value;
	int			(*on_click)(t_gui_obj *self);
	char		*str;
	size_t		str_len;
	t_gui_type	type;
	t_gui		*from;
};

t_gui_obj	gui_obj_create(
				char const *const str,
				t_gui_type const type,
				int (*on_click)(t_gui_obj *self),
				int *const value);
void		gui_obj_destroy(void *const obj);
void		gui_obj_display(
				t_engine *const eng,
				t_gui_obj *const obj,
				t_v2i const pos);

t_gui		gui_create(
				t_engine *const eng,
				t_v2i const pos,
				t_v2i const size,
				char const *const title);
void		gui_destroy(t_engine *const eng, t_gui *const gui);
t_gui_obj	*gui_add(t_gui *const gui, t_gui_obj *const obj);
void		gui_display(t_engine *const eng, t_gui *const gui);

#endif
