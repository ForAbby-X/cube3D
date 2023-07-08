/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/08 16:35:17 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"
#include "parsing.h"
#include "interface.h"

struct s_data
{
	t_map		map;
	t_sprite	*sub_screen;
	t_camera	cam;
	t_gui		gui;
	size_t		tick;
	int			show_settings;
};

//static inline void	__collision(
//	t_map *const map,
//	t_camera *const cam,
//	t_v3f const vel)
//{
	
//}

static inline void	__control(
	t_engine *const eng,
	t_camera *const cam,
	t_data *const data,
	double const dt)
{
	t_v3f const	dir = (t_v3f){cosf(cam->rot[x]), 0.0f, sinf(cam->rot[x])};
	t_v3f const	off = (t_v3f){-sinf(cam->rot[x]), 0.0f, cosf(cam->rot[x])};
	t_v3f const	up = (t_v3f){0.0f, 1.0f, 0.0f};
	t_v3f		vel;

	vel = (t_v3f){0};
	if (eng->keys[XK_w])
		vel += dir;
	if (eng->keys[XK_s])
		vel -= dir;
	if (eng->keys[XK_d])
		vel += off;
	if (eng->keys[XK_a])
		vel -= off;
	if (eng->keys[XK_space])
		vel += up;
	if (eng->keys[XK_Shift_L])
		vel -= up;
	cam->pos += v3fnorm(vel, dt * 4.0f);
	if (eng->keys[XK_Right])
		cam->rot[x] += dt * 2;
	if (eng->keys[XK_Left])
		cam->rot[x] -= dt * 2;
	if (eng->keys[XK_Down])
		cam->rot[y] -= dt * 2;
	if (eng->keys[XK_Up])
		cam->rot[y] += dt * 2;
	if (!data->show_settings)
	{
		cam->rot[x] += ((float)eng->mouse_x - 500) / 500.0f;
		cam->rot[y] -= ((float)eng->mouse_y - 260) / 500.0f;
		mlx_mouse_move(eng->mlx, eng->win, 500, 260);
	}
}

static inline int	__loop(t_engine *eng, t_data *data, double dt)
{
	if (ft_key(eng, XK_Tab).pressed)
	{
		if (data->show_settings)
		{
			mlx_mouse_move(eng->mlx, eng->win, 500, 260);
			eng->mouse_x = 500;
			eng->mouse_y = 260;
		}
		data->show_settings = !data->show_settings;
	}
	(void)dt;
	(void)__control;
	__control(eng, &data->cam, data, dt);
	if (data->show_settings)
		gui_update(eng, &data->gui);

	ft_eng_sel_spr(eng, data->sub_screen);
	ray_render(eng, &data->map, &data->cam, data->tick);
	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, data->sub_screen, (t_v2i){0}, 2);
	gui_display(eng, &data->gui);
	data->tick++;
	return (1);
}

int	main(int argc, char **argv)
{
	t_engine	*eng;
	t_data		data;

	(void)argc;
	eng = ft_eng_create(250 * 4, 130 * 4, "cube3D");
	if (eng)
	{
		data.sub_screen = ft_sprite(eng, 250 * 2, 130 * 2);
		data.map = pars_file(eng, argv[1]);
		data.gui = gui_create(eng,
				(t_v2i){10, 10}, (t_v2i){600, 400}, "Settings");
		gui_add_text(&data.gui, NULL);
		gui_add_check(&data.gui, "fog", &data.map.fog);
		gui_add_text(&data.gui, "fog color:");
		gui_add_slider(&data.gui, (t_gui_data){.u_v = &data.map.fog_color.b,
			.u_v_mi = 0, .u_v_ma = 255, .type = 2});
		gui_add_slider(&data.gui, (t_gui_data){.u_v = &data.map.fog_color.g,
			.u_v_mi = 0, .u_v_ma = 255, .type = 2});
		gui_add_slider(&data.gui, (t_gui_data){.u_v = &data.map.fog_color.r,
			.u_v_mi = 0, .u_v_ma = 255, .type = 2});
		gui_add_text(&data.gui, "fog distance:");
		gui_add_slider(&data.gui,
			(t_gui_data){.f_v = &data.map.fog_distance,
			.f_v_mi = 2.f, .f_v_ma = 20.f, .type = 0});
		gui_add_text(&data.gui, NULL);
		gui_add_text(&data.gui, "fov:");
		gui_add_slider(&data.gui, (t_gui_data){.f_v = &data.cam.fov,
			.f_v_mi = M_PI / 20.f, .f_v_ma = M_PI - M_PI / 20.f, .type = 0});
		gui_add_text(&data.gui, NULL);
		gui_add_text(&data.gui, "ground reflectivness:");
		gui_add_slider(&data.gui,
			(t_gui_data){.u_v = &data.map.sprites[4]->data[0].a,
			.u_v_mi = 0, .u_v_ma = 255, .type = 2});
		gui_add_text(&data.gui, "ceilling reflectivness:");
		gui_add_slider(&data.gui,
			(t_gui_data){.u_v = &data.map.sprites[5]->data[0].a,
			.u_v_mi = 0, .u_v_ma = 255, .type = 2});
		if (data.map.data)
		{
			data.cam = (t_camera){data.map.spawn, {0.0f, 0.0f}, M_PI_2};
			data.show_settings = 0;
			mlx_mouse_move(eng->mlx, eng->win, 500, 260);
			eng->mouse_x = 500;
			eng->mouse_y = 260;
			ft_eng_play(eng, &data, __loop);
			gui_destroy(eng, &data.gui);
			map_destroy(eng, &data.map);
		}
		else
			ft_putstr_fd("Error: Failed to initialise the game.\n", 1);
		ft_destroy_sprite(eng, data.sub_screen);
		ft_eng_destroy(eng);
	}
	return (0);
}
