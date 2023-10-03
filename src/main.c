/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/03 16:26:33 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycaster.h"
#include "parsing.h"

// #define AZERTY
#include "keys.h"
#include "game.h"
#include "minimap.h"
#include "model.h"
#include "entity.h"
#include "entity/generic.h"

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
	data->box.pos += v3fnorm(vel, dt * 2.0f);
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

static inline int	__game_init(t_engine *eng, t_data *game, char **argv)
{
	game->eng = eng;

	// XFixesHideCursor(((t_xvar *)eng->mlx)->display, ((t_win_list *)eng->win)->window);

	game->sprites[0] = ft_sprite_p(eng, "assets/block.xpm");
	game->sprites[1] = ft_sprite_p(eng, "assets/camera.xpm");
	game->sprites[2] = ft_sprite_p(eng, "assets/HEHE.xpm");

	game->minimap = ft_sprite(eng, 120, 120);
	if (game->minimap == NULL)
		return (1);

	game->selected_model = 0;
	game->models[0] = mesh_load(eng, "models/banana.obj");
	// game->models[1] = mesh_load(eng, "models/denis.obj");
	// game->models[2] = mesh_load(eng, "models/ball.obj");

	game->map = pars_file(eng, argv[1]);
	if (game->map.data == NULL)
		return (ft_destroy_sprite(eng, game->minimap), 1);

	game->menu = menu_create();
	menu_settings_create(eng, game);
	game->menu.selected = 3;

	game->cam = camera_create(eng, (t_v2i){eng->win_x / 2, eng->win_y / 2});
	game->cam.pos = game->map.spawn + (t_v3f){0.0f, 0.8f, 0.0f};
	game->cam.rot = (t_v2f){0.0f, 0.0f};
	game->cam.fog_color = (t_color){0x040018};

	game->show_settings = 0;
	game->box = (t_aabb){game->map.spawn - (t_v3f){0.16f, 0.0f, 0.16f},
	{0.32f, 0.825f, 0.32f}};
	game->sensitivity = 0.2f;

	game->entities = vector_create(sizeof(t_entity));
	t_entity entity = entity_create(game, game->map.spawn);
	vector_addback(&game->entities, &entity);

	mlx_mouse_move(eng->mlx, eng->win, 500, 260);
	eng->mouse_x = 500;
	eng->mouse_y = 260;
	return (0);
}

void	update_entities(t_data *const game, float const dt)
{
	t_entity	*ent;
	t_length	len;


	ent = vector_get(&game->entities, 0);
	len = vector_size(&game->entities);
	while (len > 0)
	{
		ent->update(ent, game, dt);
		ent++;
		len--;
	}
}

void	display_entities(t_data *const game)
{
	t_entity	*ent;
	t_length	len;


	ent = vector_get(&game->entities, 0);
	len = vector_size(&game->entities);
	while (len > 0)
	{
		ent->display(ent, game);
		ent++;
		len--;
	}
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

	update_entities(game, dt);

	if (game->show_settings)
		menu_update(eng, &game->menu);

	camera_update(&game->cam);
	ray_render(eng, &game->map, &game->cam);
	//atan2(data->cam.pos[z] - data->map.spawn[z], data->cam.pos[x] - data->map.spawn[x]) - M_PI_2
	// mesh_put(eng, &game->cam, (t_transform){{time, 0.f}, {.005f, .005f, .005f}, game->map.spawn}, &game->models[game->selected_model]);

	display_entities(game);

	ft_eng_sel_spr(eng, NULL);
	if (game->cam.fog)
		shader_apply_depth(&game->cam);
	ft_put_sprite_s(eng, game->cam.surface, (t_v2i){0}, 2);
	if (game->show_settings)
		menu_display(eng, &game->menu);
	else
	{
		minimap_display(eng, &game->map, &game->cam, game->minimap);
		ft_put_text(eng, (t_v2i){10, 120}, "[TAB] MENU", 2);
	}
	game->tick++;
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
			vector_destroy(&data.entities);
			ft_destroy_sprite(eng, data.minimap);
			ft_destroy_sprite(eng, data.sprites[0]);
			ft_destroy_sprite(eng, data.sprites[1]);
			ft_destroy_sprite(eng, data.sprites[2]);
			mesh_destroy(eng, &data.models[0]);
		}
		else
			ft_putstr_fd("Error: Failed to initialise the game.\n", 1);
		ft_eng_destroy(eng);
	}
	return (0);
}
