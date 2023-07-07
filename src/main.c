/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/07 18:17:02 by alde-fre         ###   ########.fr       */
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
	__control(eng, &data->cam, data, dt);
	ft_eng_sel_spr(eng, data->sub_screen);
	ray_render(eng, &data->map, &data->cam, data->tick);
	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, data->sub_screen, (t_v2i){0}, 2);
	if (data->show_settings)
		gui_update(eng, &data->gui);
	gui_display(eng, &data->gui);
	data->tick++;
	return (1);
}

static void	_fov_slide(t_gui_obj *const obj)
{
	float *const	fov = obj->value;

	*fov = obj->slide * (M_PI - M_PI / 10) + M_PI / 20;
}

static void	_color_slide(t_gui_obj *const obj)
{
	uint8_t *const	col = obj->value;

	*col = obj->slide * 255;
}

int	main(int argc, char **argv)
{
	t_engine	*eng;
	t_data		data;
	t_gui_obj	obj;

	(void)argc;
	eng = ft_eng_create(250 * 4, 130 * 4, "cube3D");
	if (eng)
	{
		data.sub_screen = ft_sprite(eng, 250 * 2, 130 * 2);
		data.map = pars_file(eng, argv[1]);
		data.gui = gui_create(eng,
				(t_v2i){10, 10}, (t_v2i){400, 500}, "Settings");
		obj = gui_obj_create("fog", BUTTON, NULL, &data.map.fog);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("fog color:", TEXT, NULL, NULL);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("slider", SLIDER, &_color_slide, &data.map.fog_color.b);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("slider", SLIDER, &_color_slide, &data.map.fog_color.g);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("slider", SLIDER, &_color_slide, &data.map.fog_color.r);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("", TEXT, NULL, NULL);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("FOV:", TEXT, NULL, NULL);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("slider", SLIDER, &_fov_slide, &data.cam.fov);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("", TEXT, NULL, NULL);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("GROUND TRANSPARENCY:", TEXT, NULL, NULL);
		gui_add(&data.gui, &obj);
		obj = gui_obj_create("slider", SLIDER, &_color_slide, &data.map.sprites[4]->data[0].a);
		gui_add(&data.gui, &obj);
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
