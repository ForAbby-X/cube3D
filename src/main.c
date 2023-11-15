/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 19:21:59 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "entity/entity.h"
#include "entity/all.h"
#include "particle/particle.h"
#include "aabb.h"
#include "gameplay_utils.h"

static inline void	__loop_2(t_engine *eng, t_data *game, double dt)
{
	if (game->cam.fog)
		shader_apply_depth(&game->cam);
	ft_memset(game->cam.depth_buffer, 0xFF, game->cam.surface->size[x]
		* game->cam.surface->size[y] * sizeof(float));
	if (game->state != 2)
		holding_display(eng, &game->cam, &game->models[game->selected_model],
			&game->holding);
	if (game->state != 2)
		hotbar_put(game);
	ft_eng_sel_spr(eng, game->cam.surface);
	title_update(eng, &game->title, dt);
	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, game->cam.surface, (t_v2i){0, 0}, 2);
	if (game->show_settings)
		menu_display(eng, &game->menu);
	else if (game->state != 2)
	{
		minimap_display(eng, &game->map, &game->cam, game->minimap);
		ft_put_text(eng, (t_v2i){10, 120}, "[TAB] MENU", 2);
		ft_put_nbr(eng, (t_v2i){10, 150}, 1.f / dt, 2);
		ft_put_text(eng, (t_v2i){10, 180}, "ent:", 2);
		ft_put_nbr(eng, (t_v2i){68, 180}, vector_size(&game->entities), 2);
		ft_put_text(eng, (t_v2i){10, 210}, "par:", 2);
		ft_put_nbr(eng, (t_v2i){68, 210}, vector_size(&game->particles), 2);
	}
}

static int const	g_items[] = {3, 2, 5, 8, 0, 16, 4};

static inline int	__main_menu(t_engine *eng, t_data *game, double dt)
{
	static float	stime = 0.0f;

	stime += dt;
	menu_update(eng, &game->menu);
	if (game->state == 0)
	{
		mlx_mouse_move(eng->mlx, eng->win, 500, 260);
		eng->mouse_x = 500;
		eng->mouse_y = 260;
		game->menu.selected = 3;
		ft_hide_cursor(game->eng);
	}
	game->cam.pos = (t_v3f){-1.60f, 1.5f, 0.f};
	game->cam.rot = (t_v2f){0.f, -M_PI * 0.15};
	camera_update(&game->cam);
	ft_eng_sel_spr(eng, game->cam.surface);
	ft_memset(game->cam.depth_buffer, 0xFF, game->cam.surface->size[x]
		* game->cam.surface->size[y] * sizeof(float));
	ft_clear(eng, (t_color){0x040018});
	mesh_put(eng, &game->cam, (t_transform){{stime, 0.f}, {1.f, 1.f, 1.f},
	{0.0f, -0.45f, 0.5f}}, &game->models[g_items[(int)(stime) % 7]]);
	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, game->cam.surface, (t_v2i){0, 0}, 2);
	menu_display(eng, &game->menu);
	return (game->state != -2);
}

static inline void	__death_loop(t_data *game)
{
	t_entity *const	player = game->entities.data;
	t_v3f const		dir = v3frot((t_v3f){1.f}, (t_v2f){player->rot[x], 0.3f});
	t_v3f const		center = game->death_pos - (t_v3f){0.f, .5f};
	t_ray			ray;

	player->aabb.pos = center - player->aabb.dim * 0.5f - (t_v3f){0.f, .5f};
	ray = cast_ray(&game->map, center, dir, 999.0f);
	if (ray.dist >= 1.f)
		game->cam.pos = center + dir;
	else
		game->cam.pos = ray.end - v3fnorm(dir, 0.01f);
	game->cam.rot = v3flook(game->cam.pos, center);
	player->rot[x] = player->time_alive / 6.f;
	if (player->health > 0.0f)
		player->health = 9999.f;
}

static inline int	__loop(t_engine *eng, t_data *game, double dt)
{
	int	ret;

	if (game->state == -1)
	{
		ret = __main_menu(game->eng, game, dt);
		ft_put_text(eng, (t_v2i){3, eng->sel_spr->size[y] - 28},
			"creators: vmuller/aldefre", 2);
		return (ret);
	}
	if (!game->show_settings && game->holding.click_energy < 0.1f)
		player_use_item(game, retrieve_player_hit(game));
	holding_update(&game->cam, &game->holding, dt);
	entities_update(game, dt);
	if (game->state == 2)
		__death_loop(game);
	entities_destroy(game);
	collision_ent(game, &game->entities);
	if (game->show_settings)
		menu_update(eng, &game->menu);
	camera_update(&game->cam);
	ray_render(eng, &game->map, &game->cam);
	entities_display(game);
	particles_update(game, dt);
	__loop_2(eng, game, dt);
	return (loop_la_vrai(eng, game, dt));
}

int	main(int argc, char **argv)
{
	t_engine	*eng;
	t_data		data;

	if (argc != 2)
		return (ft_putstr_fd("Usage: ./cub3D <map>\n", 1), 1);
	srand(time(NULL));
	eng = ft_eng_create(250 * 4, 230 * 3, "cube3D");
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
	else
		ft_putstr_fd("Error: Failed to initialise the engine.\n", 1);
	return (0);
}
