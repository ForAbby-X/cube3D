/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:22:15 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/13 05:02:53 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "holding.h"

void	holding_update(
			t_engine *const eng,
			t_camera *const cam,
			t_holding *const hold,
			float const dt)
{
	hold->energy_vel += v3fmag(cam->pos - hold->last_pos);
	hold->last_pos = cam->pos;
	if (ft_mouse(eng, 1).pressed)
		hold->click_energy = 0.5f;
	hold->click_energy -= dt / 1.2f;
	hold->click_energy = fmaxf(hold->click_energy, 0.0f);
}

void	holding_display(
			t_engine *const eng,
			t_camera *const cam,
			t_mesh *const model,
			t_holding *const hold)
{
	t_transform	trans;

	ft_eng_sel_spr(eng, cam->surface);
	trans.rotation = cam->rot;
	trans.rotation[x] += sinf(hold->time_from_start * M_PI / 5.f) * 0.0155f;
	trans.rotation[x] -= hold->click_energy * 0.6f;
	trans.rotation[y] += cosf(hold->time_from_start * M_PI / 3.f) * 0.08f;
	trans.rotation[y] -= hold->click_energy * 2.f;
	trans.resize = (t_v3f){0.12f, 0.12f, 0.12f};
	trans.translation = cam->pos + v3froty(
			v3frotz((t_v3f){
				0.3f + hold->click_energy * 0.8f,
				-0.12f + sinf(hold->energy_vel * 5.f) * 0.01f
				- hold->click_energy * 0.5f,
				0.2},
				cam->rot[y]), cam->rot[x]);
	mesh_put(eng, cam, trans, model);
	ft_eng_sel_spr(eng, NULL);
}
