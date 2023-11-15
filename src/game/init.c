/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 05:54:24 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 18:00:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "agrement.h"
#include "generation.h"

static int	__sprite_init(t_engine *const eng, t_data *const game)
{
	game->sprites[0] = ft_sprite_p(eng, "assets/block.xpm");
	game->sprites[1] = ft_sprite_p(eng, "assets/camera.xpm");
	game->sprites[2] = ft_sprite_p(eng, "assets/HEHE.xpm");
	game->sprites[3] = ft_sprite_p(eng, "assets/gui/hotbar/slot.xpm");
	game->sprites[4] = ft_sprite_p(eng, "assets/gui/health/full.xpm");
	game->sprites[5] = ft_sprite_p(eng, "assets/smoke_small.xpm");
	game->sprites[6] = ft_sprite_p(eng, "assets/blood.xpm");
	game->sprites[7] = ft_sprite_p(eng, "assets/spark.xpm");
	game->sprites[8] = ft_sprite_p(eng, "assets/gui/health/empty.xpm");
	game->sprites[9] = ft_sprite_p(eng, "assets/nest.xpm");
	game->sprites[10] = NULL;
	return (0);
}

static int	__models_init(t_engine *const eng, t_data *const game)
{
	game->selected_model = 4;
	game->models[0] = mesh_load(eng, "models/items/wand.obj");
	game->models[1] = mesh_load(eng, "models/projectile.obj");
	game->models[2] = mesh_load(eng, "models/items/sword.obj");
	game->models[3] = mesh_load(eng, "models/items/pickaxe.obj");
	game->models[4] = mesh_load(eng, "models/dagger.obj");
	game->models[5] = mesh_load(eng, "models/chest_close.obj");
	game->models[6] = mesh_load(eng, "models/fish.obj");
	game->models[7] = mesh_load(eng, "models/doors/single_door.obj");
	game->models[8] = mesh_load(eng, "models/scythe.obj");
	game->models[9] = mesh_load(eng, "models/gargoyle.obj");
	game->models[10] = mesh_load(eng, "models/rope.obj");
	game->models[11] = mesh_load(eng, "models/hook.obj");
	game->models[12] = mesh_load(eng, "models/man.obj");
	game->models[13] = mesh_load(eng, "models/broken_pickaxe.obj");
	game->models[14] = mesh_load(eng, "models/eye.obj");
	game->models[15] = mesh_load(eng, "models/nest.obj");
	game->models[16] = mesh_load(eng, "models/open.obj");
	game->models[17] = mesh_load(eng, "models/saddam_hussein.obj");
	game->models[18] = (t_mesh){0};
	return (0);
}

static inline int	__game_init_2(t_data *const game)
{
	game->entities = vector_create(sizeof(t_entity));
	if (game->entities.data == NULL)
		return (game_destroy(game), 1);
	game->particles = vector_create(sizeof(t_particle));
	if (game->particles.data == NULL)
		return (game_destroy(game), 1);
	e_player_add(game, game->map.spawn, game->map.spawn_dir);
	map_set(&game->map, v3ftoi(game->map.spawn) + (t_v3i){0, 1}, cell_air);
	e_hook_add(game, cast_ray(&game->map, \
		game->map.spawn, (t_v3f){0.f, 1.0f}, 99999.0f).end \
		+ (t_v3f){0.f, 0.1f});
	map_agrement(&game->map);
	map_generate(game);
	return (0);
}

int	game_init(t_engine *const eng, t_data *const game, char **argv)
{
	*game = (t_data){.eng = eng};
	game->state = -1;
	__sprite_init(eng, game);
	__models_init(eng, game);
	game->minimap = ft_sprite(eng, 120, 120);
	if (game->minimap == NULL)
		return (game_destroy(game), 1);
	game->map = pars_file(eng, (char *)argv[1]);
	if (game->map.data == NULL)
		return (game_destroy(game), 1);
	game->menu = menu_create();
	if (game->menu.data.data == NULL)
		return (game_destroy(game), 1);
	if (menu_settings_create(eng, game))
		return (game_destroy(game), 1);
	game->menu.selected = 0;
	game->cam = camera_create(eng, (t_v2i){eng->win_x / 2, eng->win_y / 2});
	game->cam.fog_color = (t_color){0x040018};
	game->holding = (t_holding){{0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f};
	game->show_settings = 0;
	game->sensitivity = 0.2f;
	game->hard_time = 0.f;
	return (__game_init_2(game) == 1);
}
