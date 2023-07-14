/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:26:08 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/14 12:24:35 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static inline void	__swap_ceilling(void *const data)
{
	t_map *const	map = data;
	t_v3i const		pos = {0, 2, 0};

	map_fill(map, pos, (t_v3i){map->size[x], 1, map->size[z]}, !map_get(map, pos));
}

static inline void	__menu_to_fog(void *const data)
{
	t_menu_id *const	sel = data;

	*sel = 0;
}

static inline void	__menu_to_control(void *const data)
{
	t_menu_id *const	sel = data;

	*sel = 1;
}

static inline void	__menu_to_player(void *const data)
{
	t_menu_id *const	sel = data;

	*sel = 2;
}

static inline void	__menu_to_settings(void *const data)
{
	t_menu_id *const	sel = data;

	*sel = 3;
}

int	menu_settings_fog_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){300, 246}, "Settings/fog");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(eng, &gui), 1);
	gui_add_button(p_gui, "<<", &__menu_to_settings, &data->menu.selected);
	gui_add_text(p_gui, NULL);
	gui_add_check(p_gui, "fog", &data->map.fog);
	gui_add_text(p_gui, "fog color:");
	gui_add_slider(p_gui, (t_gui_data){.u_v = &data->map.fog_color.b,
		.u_v_mi = 0, .u_v_ma = 255, .type = 2});
	gui_add_slider(p_gui, (t_gui_data){.u_v = &data->map.fog_color.g,
		.u_v_mi = 0, .u_v_ma = 255, .type = 2});
	gui_add_slider(p_gui, (t_gui_data){.u_v = &data->map.fog_color.r,
		.u_v_mi = 0, .u_v_ma = 255, .type = 2});
	gui_add_text(p_gui, "fog distance:");
	gui_add_slider(p_gui,
		(t_gui_data){.f_v = &data->map.fog_distance,
		.f_v_mi = 2.f, .f_v_ma = 20.f, .type = 0});
	return (0);
}

int	menu_settings_control_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){350, 290}, "Settings/controls");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(eng, &gui), 1);
	gui_add_button(p_gui, "<<", &__menu_to_settings, &data->menu.selected);
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, "move forward : W");
	gui_add_text(p_gui, "move backward : S");
	gui_add_text(p_gui, "move left : A");
	gui_add_text(p_gui, "move right : D");
	gui_add_text(p_gui, "rotate camera : MOUSE");
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, "mouse sensitivity:");
	gui_add_slider(p_gui, (t_gui_data){.f_v = &data->sensitivity,
		.f_v_mi = 0.001f, .f_v_ma = 1.0f, .type = 0});
	return (0);
}

int	menu_settings_player_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){300, 290}, "Settings/player");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(eng, &gui), 1);
	gui_add_button(p_gui, "<<", &__menu_to_settings, &data->menu.selected);
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, "player position:");
	gui_add_slider(p_gui, (t_gui_data){.f_v = ((float *)&data->box.pos),
		.f_v_mi = 0.0f, .f_v_ma = data->map.size[x] - 1.0f, .type = 0});
	gui_add_slider(p_gui, (t_gui_data){.f_v = ((float *)&data->box.pos) + 1,
		.f_v_mi = 0.0f, .f_v_ma = data->map.size[y] - 1.0f, .type = 0});
	gui_add_slider(p_gui, (t_gui_data){.f_v = ((float *)&data->box.pos) + 2,
		.f_v_mi = 0.0f, .f_v_ma = data->map.size[z] - 1.0f, .type = 0});
	gui_add_text(p_gui, "player rotation:");
	gui_add_slider(p_gui, (t_gui_data){.f_v = ((float *)&data->cam.rot),
		.f_v_mi = -M_PI, .f_v_ma = M_PI, .type = 0});
	gui_add_slider(p_gui, (t_gui_data){.f_v = ((float *)&data->cam.rot) + 1,
		.f_v_mi = -M_PI_2, .f_v_ma = M_PI_2, .type = 0});
	gui_add_text(p_gui, "fov:");
	gui_add_slider(p_gui, (t_gui_data){.f_v = &data->cam.fov,
		.f_v_mi = M_PI / 20.f, .f_v_ma = M_PI - M_PI / 20.f, .type = 0});
	return (0);
}

int	menu_settings_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	menu_settings_fog_create(eng, data);
	menu_settings_control_create(eng, data);
	menu_settings_player_create(eng, data);

	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){300, 250}, "Settings");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(eng, &gui), 1);
	gui_add_text(p_gui, NULL);
	gui_add_button(p_gui, "fog settings", &__menu_to_fog, &data->menu.selected);
	gui_add_button(p_gui, "control settings", &__menu_to_control, &data->menu.selected);
	gui_add_button(p_gui, "player settings", &__menu_to_player, &data->menu.selected);
	gui_add_text(p_gui, NULL);
	gui_add_button(p_gui, "swap ceilling", &__swap_ceilling, &data->map);
	return (0);
}

int	menu_main_create(t_engine *const eng, t_data *const data)
{
	t_gui	gui;
	t_gui	*p_gui;

	gui = gui_create(eng, (t_v2i){10, 10}, (t_v2i){300, 250}, "Cub3D (basique comme toujours)");
	if (gui.objects.data == NULL)
		return (1);
	p_gui = menu_add(&data->menu, &gui);
	if (p_gui == NULL)
		return (gui_destroy(eng, &gui), 1);
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, "play :");
	gui_add_button(p_gui, "-loaded map", &__menu_to_fog, &data->menu.selected);
	gui_add_button(p_gui, "-random labyrinth", &__menu_to_control, &data->menu.selected);
	gui_add_button(p_gui, "-random dungeon", &__menu_to_player, &data->menu.selected);
	gui_add_text(p_gui, NULL);
	gui_add_text(p_gui, NULL);
	gui_add_button(p_gui, "QUIT GAME", &__swap_ceilling, &data->map);
	return (0);
}
