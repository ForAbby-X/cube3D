/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 15:23:29 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/05 15:36:15 by alde-fre         ###   ########.fr       */
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

static t_v2i const	g_gui_offset = {2, 24};
static t_v2i const	g_gui_obj_size = {0, 22};
static t_v2i const	g_gui_obj_offset = {2, 2};

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
	t_v2i			old_mouse_pos;
	int				old_mouse_state;
	int				selected;
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
	t_v2i		pos;
	t_v2i		size;
	int			selected;
	t_v2i		old_mouse_pos;
	int			old_mouse_state;
	void		(*on_click)(t_gui_obj *self);
	void		*value;
	char		*str;
	size_t		str_len;
	t_gui_type	type;
	t_gui		*from;
};

t_gui_obj	gui_obj_create(
				char const *const str,
				t_gui_type const type,
				void (*on_click)(t_gui_obj *self),
				void *const value);
void		gui_obj_destroy(void *const obj);
void		gui_obj_update(t_engine *const eng, t_gui_obj *const obj);
void		gui_obj_display(t_engine *const eng, t_gui_obj *const obj);

t_gui		gui_create(
				t_engine *const eng,
				t_v2i const pos,
				t_v2i const size,
				char const *const title);
void		gui_destroy(t_engine *const eng, t_gui *const gui);
t_gui_obj	*gui_add(t_gui *const gui, t_gui_obj *const obj);
void		gui_update(t_engine *const eng, t_gui *const gui);
void		gui_display(t_engine *const eng, t_gui *const gui);

#endif
