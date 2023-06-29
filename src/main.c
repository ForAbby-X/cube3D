/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuller <vmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/27 18:49:09 by vmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"
#include "parsing.h"

struct s_data
{
	t_map		map;
	t_sprite	*sub_screen;
	t_camera	cam;
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
	if (eng->keys[XK_z])
		vel += dir;
	if (eng->keys[XK_s])
		vel -= dir;
	if (eng->keys[XK_d])
		vel += off;
	if (eng->keys[XK_q])
		vel -= off;
	if (eng->keys[XK_space])
		vel += up;
	if (eng->keys[XK_Shift_L])
		vel -= up;
	cam->pos += v3fnorm(vel, dt * 4.0f);
	cam->rot[x] += ((float)eng->mouse_x - 500) / 500.0f;
	cam->rot[y] -= ((float)eng->mouse_y - 260) / 500.0f;
	mlx_mouse_move(eng->mlx, eng->win, 500, 260);
}

static inline int	__loop(t_engine *eng, t_data *data, double dt)
{
	__control(eng, &data->cam, dt);
	ft_eng_sel_spr(eng, data->sub_screen);
	ray_render(eng, &data->map, &data->cam);
	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, data->sub_screen, (t_v2i){0, 0}, 4);
	return (1);
}

static inline void	__init_sprites(t_engine *const eng, t_map *const map)
{
	ft_eng_sel_spr(eng, map->sprites[0]);
	for (int y = 0; y < 128; y++)for (int x = 0; x < 128; x++)
		ft_draw(eng, (t_v2i){x, y}, ft_color(0, x, 255.0f
			- (cosf((float)y / 128.0f * M_PI * 10) / 2.0f + 0.5f)
			* (cosf((float)x / 128.0f * M_PI * 2) / 2.0f + 0.5f) * 255.0f, y));
	ft_eng_sel_spr(eng, NULL);
	ft_eng_sel_spr(eng, map->sprites[1]);
	for (int y = 0; y < 128; y++)for (int x = 0; x < 128; x++)
		ft_draw(eng, (t_v2i){x, y}, ft_color(0, x, (x * x + y * y) / 60, y));
	ft_eng_sel_spr(eng, NULL);
	ft_eng_sel_spr(eng, map->sprites[2]);
	for (int y = 0; y < 128; y++)for (int x = 0; x < 128; x++)
		ft_draw(eng, (t_v2i){x, y}, ft_color(0, y, x & y, x));
	ft_eng_sel_spr(eng, NULL);
	ft_eng_sel_spr(eng, map->sprites[3]);
	for (int y = 0; y < 128; y++)for (int x = 0; x < 128; x++)
		ft_draw(eng, (t_v2i){x, y}, ft_color(0, y, x, x));
	ft_eng_sel_spr(eng, NULL);
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
		if (data.map.data)
		{
			__init_sprites(eng, &data.map);
			data.cam = (t_camera){data.map.spawn, {0.0f, 0.0f}, M_PI_2};
			ft_eng_play(eng, &data, __loop);
			map_destroy(eng, &data.map);
		}
		else
			ft_putstr_fd("Error: Failed to initialise the game.\n", 1);
		ft_destroy_sprite(eng, data.sub_screen);
		ft_eng_destroy(eng);
	}
	return (0);
}
