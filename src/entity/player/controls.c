/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:30:00 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/21 07:19:34 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/all.h"

static inline void	__player_move(
	t_entity *const self,
	t_engine *const eng,
	t_data *const game,
	float const dt)
{
	t_v3f const	dir = {cosf(game->cam.rot[x]), 0.f, sinf(game->cam.rot[x])};
	t_v3f		vel;

	vel = (t_v3f){0};
	if (eng->keys[K_UP])
		vel += dir;
	if (eng->keys[K_DOWN])
		vel -= dir;
	if (eng->keys[K_RIGHT])
		vel += (t_v3f){-dir[z], 0.f, dir[x]};
	if (eng->keys[K_LEFT])
		vel -= (t_v3f){-dir[z], 0.f, dir[x]};
	if (eng->keys[XK_space])
		vel[y] = vel[y] + 1.f;
	if (eng->keys[XK_Shift_L])
		vel[y] = vel[y] - 1.f;
	self->vel = v3fnorm(vel, dt * 2.f);
	if (eng->keys[XK_Right])
		game->cam.rot[x] += dt * 2.f;
	if (eng->keys[XK_Left])
		game->cam.rot[x] -= dt * 2.f;
	if (eng->keys[XK_Down])
		game->cam.rot[y] -= dt * 2.f;
	if (eng->keys[XK_Up])
		game->cam.rot[y] += dt * 2.f;
}

void	player_control(
			t_entity *const self,
			t_data *const game,
			double const dt)
{
	__player_move(self, game->eng, game, dt);
	if (!game->show_settings)
	{
		game->cam.rot[x] += ((float)game->eng->mouse_x - 500)
			* (game->sensitivity / 100.f * game->cam.fov_ratio);
		game->cam.rot[y] -= ((float)game->eng->mouse_y - 260)
			* (game->sensitivity / 100.f * game->cam.fov_ratio);
		mlx_mouse_move(game->eng->mlx, game->eng->win, 500, 260);
	}
	if (game->cam.rot[x] < -M_PI)
		game->cam.rot[x] += M_PI * 2;
	else if (game->cam.rot[x] > M_PI)
		game->cam.rot[x] -= M_PI * 2;
	if (game->cam.rot[y] < -M_PI_2)
		game->cam.rot[y] = -M_PI_2;
	else if (game->cam.rot[y] > M_PI_2)
		game->cam.rot[y] = M_PI_2;
	if (ft_key(game->eng, XK_Tab).pressed)
	{
		if (game->show_settings)
		{
			mlx_mouse_move(game->eng->mlx, game->eng->win, 500, 260);
			game->eng->mouse_x = 500;
			game->eng->mouse_y = 260;
			ft_hide_cursor(game->eng);
		}
		else
			ft_show_cursor(game->eng);
		game->show_settings = !game->show_settings;
	}
	if (ft_mouse(game->eng, 1).pressed && game->selected_model == 0
		&& !game->show_settings)
		e_fireball_add(game, game->cam.pos + v3froty(v3frotz((t_v3f){0.2f},
					game->cam.rot[y]), game->cam.rot[x]), game->cam.rot);
}
