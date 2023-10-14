/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/14 23:21:37 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "entity/entity.h"
#include "entity/all.h"
#include "particle/particle.h"

static inline void	__control(
	t_engine *const eng,
	t_camera *const cam,
	t_data *const game,
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
	game->box.pos += v3fnorm(vel, dt * 2.0f);
	if (eng->keys[XK_Right])
		cam->rot[x] += dt * 2;
	if (eng->keys[XK_Left])
		cam->rot[x] -= dt * 2;
	if (eng->keys[XK_Down])
		cam->rot[y] -= dt * 2;
	if (eng->keys[XK_Up])
		cam->rot[y] += dt * 2;
	if (!game->show_settings)
	{
		cam->rot[x] += ((float)eng->mouse_x - 500)
			* (game->sensitivity / 100.f * game->cam.fov_ratio);
		cam->rot[y] -= ((float)eng->mouse_y - 260)
			* (game->sensitivity / 100.f * game->cam.fov_ratio);
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
		if (game->show_settings)
		{
			mlx_mouse_move(eng->mlx, eng->win, 500, 260);
			eng->mouse_x = 500;
			eng->mouse_y = 260;
			ft_hide_cursor(game->eng);
		}
		else
			ft_show_cursor(game->eng);
		game->show_settings = !game->show_settings;
	}
	if (ft_mouse(eng, 1).pressed)
		e_fireball_add(game, game->cam.pos, game->cam.rot);
}

static inline int	__loop(t_engine *eng, t_data *game, double dt)
{
	static float	time = 0.f;

	time += dt;
	__control(eng, &game->cam, game, dt);
	player_collision(&game->map, &game->box);
	game->cam.pos = game->box.pos;
	game->cam.pos[x] += game->box.dim[x] / 2.0f;
	game->cam.pos[y] += game->box.dim[y] * 0.8f;
	game->cam.pos[z] += game->box.dim[z] / 2.0f;

	entities_update(game, dt);
	entities_destroy(game);

	holding_update(eng, &game->cam, &game->holding, dt);

	if (game->show_settings)
		menu_update(eng, &game->menu);

	game->cam.pos[y] += sinf(game->holding.energy_vel * 5.f) * 0.03f;
	camera_update(&game->cam);
	ray_render(eng, &game->map, &game->cam);
	//atan2(data->cam.pos[z] - data->map.spawn[z], data->cam.pos[x] - data->map.spawn[x]) - M_PI_2
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){0.0f, .125f, 0.f}}, &game->models[0]);
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){0.25f, .125f, 0.f}}, &game->models[2]);
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){0.5f, .125f, 0.f}}, &game->models[3]);
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){0.75f, .125f, 0.f}}, &game->models[4]);
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){1.0f, .125f, 0.f}}, &game->models[5]);

	put_3d_spr(eng, &game->cam, game->sprites[0], game->map.spawn + (t_v3f){1.0f, .8f, 0.f});

	entities_display(game);
	particles_update(game, dt);

	if (game->cam.fog)
		shader_apply_depth(&game->cam);
	ft_memset(game->cam.depth_buffer, 0xFF, game->cam.surface->size[x]
		* game->cam.surface->size[y] * sizeof(float));
	holding_display(eng, &game->cam, &game->models[game->selected_model], &game->holding);

	hotbar_put(game);
	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, game->cam.surface, (t_v2i){0}, 2);
	if (game->show_settings)
		menu_display(eng, &game->menu);
	else
	{
		minimap_display(eng, &game->map, &game->cam, game->minimap);
		ft_put_text(eng, (t_v2i){10, 120}, "[TAB] MENU", 2);
		ft_put_nbr(eng, (t_v2i){10, 150}, vector_size(&game->entities), 2);
		ft_put_nbr(eng, (t_v2i){10, 180}, vector_size(&game->particles), 2);
	}
	game->tick++;
	game->holding.time_from_start += dt;
	time += dt;
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
		if (!game_init(eng, &data, argv))
		{
			ft_eng_play(eng, &data, __loop);
			game_destroy(&data);
		}
		else
			ft_putstr_fd("Error: Failed to initialise the game.\n", 1);
		ft_eng_destroy(eng);
	}
	return (0);
}
