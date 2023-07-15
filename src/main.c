/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/15 23:10:47 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"
#include "parsing.h"

#define AZERTY
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
		cam->rot[x] += ((float)eng->mouse_x - 500) * (data->sensitivity / 100.f);
		cam->rot[y] -= ((float)eng->mouse_y - 260) * (data->sensitivity / 100.f);
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
		
}

static inline int	__game_init(t_engine *eng, t_data *data, char **argv)
{
	data->eng = eng;
	data->sub_screen = ft_sprite(eng, 250 * 2, 130 * 2);
	if (data->sub_screen == NULL)
		return (1);
	data->depth_buffer = malloc(250 * 2 * 130 * 2 * sizeof(float));
	data->sprites[0] = ft_sprite_p(eng, "assets/HEHE.xpm");
	data->minimap = ft_sprite(eng, 120, 120);
	if (data->minimap == NULL)
		return (1);
	data->map = pars_file(eng, argv[1]);
	if (data->map.data == NULL)
		return (ft_destroy_sprite(eng, data->sub_screen), 1);
	data->menu = menu_create();
	menu_settings_create(eng, data);
	data->menu.selected = 3;
	data->cam = (t_camera){data->map.spawn + (t_v3f){0.0f, 0.8f, 0.0f}, {0.0f}, M_PI_2};
	data->show_settings = 0;
	data->box = (t_aabb){data->map.spawn - (t_v3f){0.16f, 0.0f, 0.16f},
	{0.32f, 0.825f, 0.32f}};
	data->sensitivity = 0.2f;
	mlx_mouse_move(eng->mlx, eng->win, 500, 260);
	eng->mouse_x = 500;
	eng->mouse_y = 260;
	return (0);
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
	player_collision(&data->map, &data->box);
	data->cam.pos = data->box.pos;
	data->cam.pos[x] += data->box.dim[x] / 2.0f;
	data->cam.pos[y] += data->box.dim[y] * 0.8f;
	data->cam.pos[z] += data->box.dim[z] / 2.0f;
	if (data->show_settings)
		menu_update(eng, &data->menu);
	ft_eng_sel_spr(eng, data->sub_screen);
	ray_render(data, data->tick);

	put_3d_point(data, data->box.pos);
	put_3d_point(data, data->box.pos + (t_v3f){data->box.dim[x], 0.f, 0.f});
	put_3d_point(data, data->box.pos + (t_v3f){0.f, 0.f, data->box.dim[z]});
	put_3d_point(data, data->box.pos + (t_v3f){data->box.dim[x], 0.f, data->box.dim[z]});
	put_3d_point(data, data->box.pos + (t_v3f){0.f, data->box.dim[y], 0.f});
	put_3d_point(data, data->box.pos + (t_v3f){data->box.dim[x], data->box.dim[y], 0.f});
	put_3d_point(data, data->box.pos + (t_v3f){0.f, data->box.dim[y], data->box.dim[z]});
	put_3d_point(data, data->box.pos + data->box.dim);

	for (int i = 0; i < 50; i++)
		put_3d_spr(data, data->sprites[0], data->map.spawn + (t_v3f){sinf(i / 50.f * M_PI * 2) / 2.f, 0.2f, cosf(i / 50.f * M_PI * 2) / 2.f});

	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, data->sub_screen, (t_v2i){0}, 2);
	if (data->show_settings)
		menu_display(eng, &data->menu);
	else
		minimap_display(eng, &data->map, &data->cam, data->minimap);
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
			ft_destroy_sprite(eng, data.sub_screen);
		}
		else
			ft_putstr_fd("Error: Failed to initialise the game.\n", 1);
		ft_eng_destroy(eng);
	}
	return (0);
}
