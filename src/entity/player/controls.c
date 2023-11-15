/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:30:00 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 17:58:01 by alde-fre         ###   ########.fr       */
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
	self->vel = v3fnorm(vel, dt * 2.f);
}

static inline void	__player_rotate(
	t_entity *const self,
	t_engine *const eng,
	t_data *const game,
	float const dt)
{
	if (eng->keys[XK_Right])
		self->rot[x] += dt * 2.f;
	if (eng->keys[XK_Left])
		self->rot[x] -= dt * 2.f;
	if (eng->keys[XK_Down])
		self->rot[y] -= dt * 2.f;
	if (eng->keys[XK_Up])
		self->rot[y] += dt * 2.f;
	if (!game->show_settings)
	{
		self->rot[x] += ((float)game->eng->mouse_x - 500)
			* (game->sensitivity / 100.f);
		self->rot[y] -= ((float)game->eng->mouse_y - 260)
			* (game->sensitivity / 100.f);
		mlx_mouse_move(game->eng->mlx, game->eng->win, 500, 260);
	}
	if (self->rot[y] < -M_PI_2)
		self->rot[y] = -M_PI_2;
	else if (self->rot[y] > M_PI_2)
		self->rot[y] = M_PI_2;
}

void	player_control(
			t_entity *const self,
			t_data *const game,
			double const dt)
{
	if (game->state != 2)
	{
		__player_move(self, game->eng, game, dt);
		__player_rotate(self, game->eng, game, dt);
	}
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
}
