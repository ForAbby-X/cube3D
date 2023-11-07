/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:36:00 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/07 05:55:27 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "entity/entity.h"
#include "entity/all.h"
#include "particle/particle.h"
#include "aabb.h"

static inline int	__loop(t_engine *eng, t_data *game, double dt)
{
	static float	time = 0.f;

	time += dt;

	game->cam.pos = ((t_entity *)game->entities.data)->aabb.pos
		+ (t_v3f){0.16f, 0.7f, 0.16f};
	entities_update(game, dt);

	entities_collisions(game);
	collision_ent(&game->entities, &game->map);

	entities_destroy(game);

	holding_update(eng, &game->cam, &game->holding, dt); // TO DO IN PLAYER AI

	if (game->show_settings)
		menu_update(eng, &game->menu);

	t_ray	ray = cast_ray(&game->map, game->cam.pos, v3froty(v3frotz((t_v3f){1.f}, game->cam.rot[y]), game->cam.rot[x]), 999);
	if (!game->show_settings && ft_mouse(eng, 1).pressed
		&& game->selected_model == 3
		&& map_get(&game->map, ray.pos) == cell_wall)
	{
		p_block_add(game, v3itof(ray.pos) + (t_v3f){.5f, .5f, .5f});
		map_set(&game->map, ray.pos, cell_air);
	}

	game->cam.pos[y] += sinf(game->holding.energy_vel * 5.f) * 0.03f;
	camera_update(&game->cam);
	ray_render(eng, &game->map, &game->cam);

	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){0.0f, .125f, 0.f}}, &game->models[0]);
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){0.25f, .125f, 0.f}}, &game->models[2]);
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){0.5f, .125f, 0.f}}, &game->models[3]);
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){0.75f, .125f, 0.f}}, &game->models[4]);
	mesh_put(eng, &game->cam, (t_transform){{time, 0.25f}, {.125f, .125f, .125f}, game->map.spawn + (t_v3f){1.0f, .125f, 0.f}}, &game->models[5]);

	entities_display(game);
	particles_update(game, dt);

	if (game->cam.fog)
		shader_apply_depth(&game->cam);
	ft_memset(game->cam.depth_buffer, 0xFF, game->cam.surface->size[x]
		* game->cam.surface->size[y] * sizeof(float));
	holding_display(eng, &game->cam, &game->models[game->selected_model], &game->holding);

	hotbar_put(game);
	ft_circle(eng, eng->sel_spr->size / 2, 2, (t_color){0xFFFFFF});
	ft_eng_sel_spr(eng, NULL);
	ft_put_sprite_s(eng, game->cam.surface, (t_v2i){0, 0}, 2);
	if (game->show_settings)
		menu_display(eng, &game->menu);
	else
	{
		minimap_display(eng, &game->map, &game->cam, game->minimap);
		ft_put_text(eng, (t_v2i){10, 120}, "[TAB] MENU", 2);
		ft_put_nbr(eng, (t_v2i){10, 150}, 1.f / dt, 2);
		ft_put_text(eng, (t_v2i){10, 180}, "ent:", 2);
		ft_put_nbr(eng, (t_v2i){68, 180}, vector_size(&game->entities), 2);
		ft_put_text(eng, (t_v2i){10, 210}, "par:", 2);
		ft_put_nbr(eng, (t_v2i){68, 210}, vector_size(&game->particles), 2);
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
