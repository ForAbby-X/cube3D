/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 15:23:29 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/13 23:28:40 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

# include "engine.h"
# include "vec3.h"

typedef struct s_gui_container	t_gui;
typedef enum e_gui_type			t_gui_type;
typedef struct s_gui_data		t_gui_data;
typedef struct s_gui_obj		t_gui_obj;
typedef void					(*t_obj_meth)(void *const data);

static t_v2i const	g_gui_offset = {2, 24};
static t_v2i const	g_gui_obj_size = {0, 22};
static t_v2i const	g_gui_obj_offset = {2, 2};

struct s_gui_container
{
	t_v2i			pos;
	t_v2i			size;
	t_v2i			old_mouse_pos;
	int				selected;
	t_vector		objects;
	char			*title;
	size_t			title_len;
	int				fixed;
	t_sprite		*surface;
};

enum e_gui_type
{
	CHECK,
	SLIDER,
	TEXT,
	BUTTON
};

struct s_gui_data
{
	union {
		float	*f_v;
		int		*i_v;
		uint8_t	*u_v;
	};
	union {
		float	f_v_mi;
		int		i_v_mi;
		uint8_t	u_v_mi;
	};
	union {
		float	f_v_ma;
		int		i_v_ma;
		uint8_t	u_v_ma;
	};
	uint8_t	type;
};

struct s_gui_obj
{
	t_v2i		pos;
	t_v2i		size;
	int			selected;
	int			hovered;
	t_v2i		old_mouse_pos;
	t_obj_meth	on_click;
	void		*on_click_data;
	int			*check_data;
	t_gui_data	data;
	float		slide;
	char		*str;
	size_t		str_len;
	int			visible;
	t_gui_type	type;
	t_gui		*from;
};

t_gui_obj	gui_obj_create(
				char const *const str,
				t_gui_type const type,
				t_obj_meth on_click,
				t_gui_data data);
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

t_gui_obj	*gui_add_text(t_gui *const gui, char const *const str);
t_gui_obj	*gui_add_check(
				t_gui *const gui,
				char const *const str,
				int *const on_click_data);
t_gui_obj	*gui_add_slider(t_gui *const gui, t_gui_data data);
t_gui_obj	*gui_add_button(
				t_gui *const gui,
				char const *const str,
				t_obj_meth on_click,
				void *const on_click_data);

void		gui_update(t_engine *const eng, t_gui *const gui);
void		gui_display(t_engine *const eng, t_gui *const gui);

#endif
