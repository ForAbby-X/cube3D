/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/01 04:52:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"
#include "parsing.h"

// #define AZERTY
#include "keys.h"
#include "game.h"
#include "minimap.h"
#include "model.h"

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
	if (eng->keys[K_UP])
		vel += dir;
	if (eng->keys[K_DOWN])
		vel -= dir;
	if (eng->keys[K_RIGHT])
		vel += off;
	if (eng->keys[K_LEFT])
		vel -= off;
	if (eng->keys[XK_space])
		vel += up;
	if (eng->keys[XK_Shift_L])
		vel -= up;
	data->box.pos += v3fnorm(vel, dt * 4.0f);
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
		cam->rot[x] += ((float)eng->mouse_x - 500)
			* (data->sensitivity / 100.f * data->cam.fov_ratio);
		cam->rot[y] -= ((float)eng->mouse_y - 260)
			* (data->sensitivity / 100.f * data->cam.fov_ratio);
		mlx_mouse_move(eng->mlx, eng->win, 500, 260);
	}
	if (cam->rot[x] < -M_PI)
		cam->rot[x] += M_PI * 2;
	else if (cam->rot[x] > M_PI)
		cam->rot[x] -= M_PI * 2;
	if (cam->rot[y] < -M_PI_2)
		cam->rot[y] = -M_PI_2;
	else if (cam->rot[y] > M_PI_2)
		cam->rot[y] = M_PI_2;
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
}

static inline int	__game_init(t_engine *eng, t_data *data, char **argv)
{
	data->eng = eng;

	// XFixesHideCursor(((t_xvar *)eng->mlx)->display, ((t_win_list *)eng->win)->window);

	data->sprites[0] = ft_sprite_p(eng, "assets/block.xpm");
	data->sprites[1] = ft_sprite_p(eng, "assets/camera.xpm");
	data->sprites[2] = ft_sprite_p(eng, "assets/HEHE.xpm");

	data->minimap = ft_sprite(eng, 120, 120);
	if (data->minimap == NULL)
		return (1);

	data->selected_model = 0;
	data->models[0] = mesh_load(eng, "models/fabienne.obj");
	data->models[1] = mesh_load(eng, "models/denis.obj");
	data->models[2] = mesh_load(eng, "models/ball.obj");

	data->map = pars_file(eng, argv[1]);
	if (data->map.data == NULL)
		return (ft_destroy_sprite(eng, data->minimap), 1);

	data->menu = menu_create();
	menu_settings_create(eng, data);
	data->menu.selected = 3;

	data->cam = camera_create(eng, (t_v2i){eng->win_x / 2, eng->win_y / 2});
	data->cam.pos = data->map.spawn + (t_v3f){0.0f, 0.8f, 0.0f};
	data->cam.rot = (t_v2f){0.0f, 0.0f};
	data->cam.fog_color = (t_color){0x040018};

	data->show_settings = 0;
	data->box = (t_aabb){data->map.spawn - (t_v3f){0.16f, 0.0f, 0.16f},
	{0.32f, 0.825f, 0.32f}};
	data->sensitivity = 0.2f;

	data->points[0] = (t_v3f){0};
	data->points[1] = (t_v3f){0};
	data->selected = 0;

	mlx_mouse_move(eng->mlx, eng->win, 500, 260);
	eng->mouse_x = 500;
	eng->mouse_y = 260;
	return (0);
}

static inline int	__loop(t_engine *eng, t_data *data, double dt)
{
	static float	time = 0.f;

	time += dt;
	__control(eng, &data->cam, data, dt);
	player_collision(&data->map, &data->box);
	data->cam.pos = data->box.pos;
	data->cam.pos[x] += data->box.dim[x] / 2.0f;
	data->cam.pos[y] += data->box.dim[y] * 0.8f;
	data->cam.pos[z] += data->box.dim[z] / 2.0f;

	if (data->show_settings)
		menu_update(eng, &data->menu);

	camera_update(&data->cam);
	ray_render(eng, &data->map, &data->cam);

	mesh_put(eng, &data->cam, (t_transform){{atan2(data->box.pos[z] - data->map.spawn[z], data->box.pos[x] - data->map.spawn[x]) - M_PI_2, 0.f}, {.005f, .005f, .005f}, data->map.spawn}, &data->models[data->selected_model]);

	ft_eng_sel_spr(eng, NULL);
	if (data->cam.fog)
		shader_apply_depth(&data->cam);
	ft_put_sprite_s(eng, data->cam.surface, (t_v2i){0}, 2);
	if (data->show_settings)
		menu_display(eng, &data->menu);
	else
	{
		minimap_display(eng, &data->map, &data->cam, data->minimap);
		ft_put_text(eng, (t_v2i){10, 120}, "[TAB] MENU", 2);
	}
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
		if (!__game_init(eng, &data, argv))
		{
			ft_eng_play(eng, &data, __loop);
			menu_destroy(eng, &data.menu);
			map_destroy(eng, &data.map);
			camera_destroy(eng, &data.cam);
		}
		else
			ft_putstr_fd("Error: Failed to initialise the game.\n", 1);
		ft_eng_destroy(eng);
	}
	return (0);
}
