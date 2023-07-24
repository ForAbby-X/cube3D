/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 01:40:17 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/24 15:46:24 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

int	ft_abs(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

static inline void	__draw_line(
		t_engine *const eng,
		t_v2i const f,
		t_v2i const s,
		t_color const color)
{
	t_v2i const	delta = {ft_abs(s[x] - f[x]), ft_abs(s[y] - f[y])};
	t_v2i		sign;
	t_v2i		cur;
	int			error[2];

	sign[x] = f[x] < s[x] ? 1 : -1;
	sign[y] = f[y] < s[y] ? 1 : -1;
	error[0] = delta[x] - delta[y];
	cur = f;
	while (cur[x] != s[x] || cur[y] != s[y])
	{
		ft_draw(eng, cur, color);
		error[1] = error[0] * 2;
		if (error[1] > -delta[y])
		{
			error[0] -= delta[y];
			cur[x] += sign[x];
		}
		if (error[1] < delta[x])
		{
			error[0] += delta[x];
			cur[y] += sign[y];
		}
	}
}

static inline void	__triangle_put(t_engine *const eng, t_v2i const pos[3])
{
	__draw_line(eng, pos[0], pos[1], (t_color){0xEFEFEF});
	__draw_line(eng, pos[1], pos[2], (t_color){0xEFEFEF});
	__draw_line(eng, pos[2], pos[0], (t_color){0xEFEFEF});
}

static inline void	__polygon_to_pix(t_v3f const pos[3], t_v2i pix[3])
{
	pix[0] = (t_v2i){pos[0][x], pos[0][y]};
	pix[1] = (t_v2i){pos[1][x], pos[1][y]};
	pix[2] = (t_v2i){pos[2][x], pos[2][y]};
}

void	mesh_put(
			t_engine *const eng,
			t_camera *const cam,
			t_v3f const pos,
			t_mesh *const mesh)
{
	t_polygon	polygon;
	t_v3f		vertex[3];
	t_length	index;
	t_v2i		pix[3];

	ft_eng_sel_spr(eng, cam->surface);
	index = 0;
	while (index < vector_size(&mesh->polygons))
	{
		polygon = *((t_polygon *)vector_get(&mesh->polygons, index));

		polygon.points[0] *= 0.02f;
		polygon.points[1] *= 0.02f;
		polygon.points[2] *= 0.02f;

		polygon.points[0] += pos;
		polygon.points[1] += pos;
		polygon.points[2] += pos;
		vertex[0] = project_point(polygon.points[0], cam);
		vertex[1] = project_point(polygon.points[1], cam);
		vertex[2] = project_point(polygon.points[2], cam);
		__polygon_to_pix(vertex, pix);
		__triangle_put(eng, pix);
		index++;
	}
	ft_eng_sel_spr(eng, NULL);
}
