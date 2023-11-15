/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 05:24:51 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/13 12:58:14 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity/entity.h"

static void	draw_line(t_engine *eng, t_camera *cam, t_v3f start, t_v3f end)
{
	int const	steps = 10;
	t_v3f const	diff = end - start;
	float		t;
	int			i;

	i = 0;
	while (i <= steps)
	{
		t = (float)i / steps;
		put_3d_point(eng, cam, start + t * diff, 0.005f);
		++i;
	}
}

static void	display_aabb_3d(t_engine *eng, t_camera *cam, t_aabb *aabb)
{
	t_v3f		corners[8];
	int			i;
	int const	edges[12][2] = {{0, 1}, {1, 3}, {3, 2}, {2, 0}, {4, 5}, \
	{5, 7}, {7, 6}, {6, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

	i = 0;
	while (i < 8)
	{
		corners[i][0] = aabb->pos[0] + (i & 1) * aabb->dim[0];
		corners[i][1] = aabb->pos[1] + ((i >> 1) & 1) * aabb->dim[1];
		corners[i][2] = aabb->pos[2] + ((i >> 2) & 1) * aabb->dim[2];
		i++;
	}
	i = -1;
	while (++i < 12)
		draw_line(eng, cam, corners[edges[i][0]], corners[edges[i][1]]);
}

void	entities_update(t_data *const game, float const dt)
{
	t_entity	*ent;
	t_length	index;

	index = 0;
	while (index < game->entities.size)
	{
		ent = &((t_entity *)game->entities.data)[index];
		ent->update(ent, game, dt);
		ent->time_alive += dt;
		++index;
	}
}

void	entities_display(t_data *const game)
{
	t_entity	*ent;
	t_length	index;

	index = 0;
	while (index < game->entities.size)
	{
		ent = &((t_entity *)game->entities.data)[index];
		ent->display(ent, game);
		if (game->show_settings)
			display_aabb_3d(game->eng, &game->cam, &ent->aabb);
		++index;
	}
}

void	entities_destroy(t_data *const game)
{
	t_entity	*ent;
	t_length	index;

	index = 0;
	while (index < game->entities.size)
	{
		ent = &((t_entity *)game->entities.data)[index];
		if (ent->dead)
		{
			ent->destroy(ent, game);
			vector_erase(&game->entities, index);
		}
		else
			index++;
	}
}
