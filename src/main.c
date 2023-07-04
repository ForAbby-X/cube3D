/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/04 12:59:11 by alde-fre         ###   ########.fr       */
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
	// cam->rot[x] += ((float)eng->mouse_x - 500) / 500.0f;
	// cam->rot[y] -= ((float)eng->mouse_y - 260) / 500.0f;
	// mlx_mouse_move(eng->mlx, eng->win, 500, 260);
}

static inline int	__loop(t_engine *eng, t_data *data, double dt)
{
	static float	time;

	time += dt;
	__control(eng, &data->cam, dt);
	ft_eng_sel_spr(eng, data->sub_screen);
	ray_render(eng, &data->map, &data->cam, time);
	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, data->sub_screen, (t_v2i){0, 0}, 4);
	gui_display(eng, &data->gui);
	return (1);
}

static inline void	__init_sprites(t_engine *const eng, t_map *const map)
{
	t_color	color;

	for (int i = 0; i < 6; i++)
	{
		ft_eng_sel_spr(eng, map->sprites[i]);
		for (int y = 0; y < (int)ft_eng_size_y(eng); y++)
		{
			for (int x = 0; x < (int)ft_eng_size_x(eng); x++)
			{
				color = ft_get_color(eng->sel_spr, (t_v2i){x, y});
				color.a = ft_color_med(color) >> 1;
				ft_draw(eng, (t_v2i){x, y}, color);
			}
		}
		ft_eng_sel_spr(eng, NULL);
	}
}
int	main(int argc, char **argv)
{
	t_engine	*eng;
	t_data		data;

	(void)argc;
	eng = ft_eng_create(250 * 4, 130 * 4, "cube3D");
	if (eng)
	{
		data.sub_screen = ft_sprite(eng, 250, 130);
		data.map = pars_file(eng, argv[1]);
		data.gui = gui_create(eng,
				(t_v2i){0, 0}, (t_v2i){400, 230}, "Settings");
		if (data.map.data)
		{
			(void)__init_sprites;
			data.cam = (t_camera){data.map.spawn, {0.0f, 0.0f}, M_PI_2};
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
