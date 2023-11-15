/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:26:08 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 17:42:04 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static inline void	menu_settings_fog_create2(
					t_data *const data,
					t_gui	*const p_gui)
{
	gui_add_button(p_gui, "<<", &__menu_to_settings, &data->menu.selected);
	gui_add_text(p_gui, NULL);
	gui_add_check(p_gui, "fog", &data->cam.fog);
	gui_add_text(p_gui, "fog color:");
	gui_add_slider(p_gui, (t_gui_data){.u_v = &data->cam.fog_color.b,
		.u_v_mi = 0, .u_v_ma = 255, .type = 2});
	gui_add_slider(p_gui, (t_gui_data){.u_v = &data->cam.fog_color.g,
		.u_v_mi = 0, .u_v_ma = 255, .type = 2});
	gui_add_slider(p_gui, (t_gui_data){.u_v = &data->cam.fog_color.r,
		.u_v_mi = 0, .u_v_ma = 255, .type = 2});
	gui_add_text(p_gui, "fog distance:");
	gui_add_slider(p_gui,
		(t_gui_data){.f_v = &data->cam.fog_distance,
		.f_v_mi = 2.f, .f_v_ma = 40.f, .type = 0});
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, "fov:");
	gui_add_slider(p_gui, (t_gui_data){.f_v = &data->cam.fov,
		.f_v_mi = M_PI / 20.f, .f_v_ma = M_PI - M_PI / 20.f, .type = 0});
}

int	menu_settings_fog_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){300, 314},
			"Settings/camera");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(&gui), 1);
	menu_settings_fog_create2(data, p_gui);
	return (0);
}

int	menu_settings_control_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){350, 290},
			"Settings/controls");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(&gui), 1);
	gui_add_button(p_gui, "<<", &__menu_to_settings, &data->menu.selected);
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, "move forward  : W");
	gui_add_text(p_gui, "move backward : S");
	gui_add_text(p_gui, "move left     : A");
	gui_add_text(p_gui, "move right    : D");
	gui_add_text(p_gui, "rotate camera : MOUSE");
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, "mouse sensitivity:");
	gui_add_slider(p_gui, (t_gui_data){.f_v = &data->sensitivity,
		.f_v_mi = 0.001f, .f_v_ma = 1.0f, .type = 0});
	return (0);
}

int	menu_settings_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	menu_main_create(eng, data);
	menu_settings_fog_create(eng, data);
	menu_settings_control_create(eng, data);
	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){300, 320}, "Settings");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(&gui), 1);
	gui_add_text(p_gui, NULL);
	gui_add_button(p_gui, "camera settings",
		&__menu_to_fog, &data->menu.selected);
	gui_add_button(p_gui, "control settings",
		&__menu_to_control, &data->menu.selected);
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, "selected model:");
	gui_add_slider(p_gui,
		(t_gui_data){.i_v = &data->selected_model,
		.i_v_mi = 0, .i_v_ma = 7, .type = 1});
	return (0);
}

int	menu_main_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){200, 140},
			"Cub3D");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(&gui), 1);
	gui_add_text(p_gui, NULL);
	gui_add_button(p_gui, "PLAY MAP", &__menu_launch, &data->state);
	gui_add_text(p_gui, NULL);
	gui_add_button(p_gui, "QUIT GAME", &__menu_quit, &data->state);
	return (0);
}
