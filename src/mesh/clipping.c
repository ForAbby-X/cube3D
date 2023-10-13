/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 15:21:11 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/13 07:53:38 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

typedef struct s_clip
{
	t_vert	vert;
	t_xyz	index;
}	t_clip;

static inline void	__clipping_of_tri_1(
						t_vector *const to_clip,
						t_clip io_points[2][3],
						t_v3f const plane,
						t_xyz axis)
{
	t_polygon	corr;

	enum e_minmax {in, out};
	(void)to_clip;
	corr.vert[io_points[in][0].index] = io_points[in][0].vert;
	corr.vert[io_points[in][1].index] = io_points[in][1].vert;
	corr.vert[io_points[out][0].index] = inter_ray_pol(plane, axis,
			io_points[in][0].vert, io_points[out][0].vert);
	vector_addback(to_clip, &corr);
	corr.vert[io_points[in][0].index] = inter_ray_pol(plane, axis,
			io_points[in][1].vert, io_points[out][0].vert);
	vector_addback(to_clip, &corr);
}

static inline void	__clipping_of_tri_2(
						t_vector *const to_clip,
						t_clip io_points[2][3],
						t_v3f const plane,
						t_xyz axis)
{
	t_vert	corr[3];

	enum e_minmax {in, out};
	corr[io_points[in][0].index] = io_points[in][0].vert;
	corr[io_points[out][0].index] = inter_ray_pol(plane, axis,
			io_points[in][0].vert, io_points[out][0].vert);
	corr[io_points[out][1].index] = inter_ray_pol(plane, axis,
			io_points[in][0].vert, io_points[out][1].vert);
	vector_addback(to_clip, corr);
}

int	clip_poly_near(
			t_polygon const *const poly,
			t_vector *const to_clip,
			t_camera *const cam,
			int const type)
{
	t_clip		io_points[2][3];
	t_length	io_nb[2];
	int			ind;
	t_xyz		axis;

	enum e_minmax {in, out};
	axis = (t_xyz [2]){-1, y}[type];
	while (++axis <= (t_xyz [2]){y, z}[type])
	{
		io_nb[0] = 0;
		io_nb[1] = 0;
		ind = poly->vert[0].point[axis] < cam->fru_near[axis];
		io_points[ind][io_nb[ind]++] = (t_clip){poly->vert[0], 0};
		ind = poly->vert[1].point[axis] < cam->fru_near[axis];
		io_points[ind][io_nb[ind]++] = (t_clip){poly->vert[1], 1};
		ind = poly->vert[2].point[axis] < cam->fru_near[axis];
		io_points[ind][io_nb[ind]++] = (t_clip){poly->vert[2], 2};
		if (io_nb[1] == 1)
			__clipping_of_tri_1(to_clip, io_points, cam->fru_near, axis);
		else if (io_nb[1] == 2)
			__clipping_of_tri_2(to_clip, io_points, cam->fru_near, axis);
		if (io_nb[1] == 1 || io_nb[1] == 2 || io_nb[1] == 3)
			return (0);
	}
	return (1);
}

int	clip_poly_far(
			t_polygon const *const poly,
			t_vector *const to_clip,
			t_camera *const cam,
			int const type)
{
	t_clip		io_points[2][3];
	t_length	io_nb[2];
	int			ind;
	t_xyz		axis;

	enum e_minmax {in, out};
	axis = (t_xyz [2]){-1, y}[type];
	while (++axis <= (t_xyz [2]){y, z}[type])
	{
		io_nb[0] = 0;
		io_nb[1] = 0;
		ind = poly->vert[0].point[axis] > cam->fru_far[axis];
		io_points[ind][io_nb[ind]++] = (t_clip){poly->vert[0], 0};
		ind = poly->vert[1].point[axis] > cam->fru_far[axis];
		io_points[ind][io_nb[ind]++] = (t_clip){poly->vert[1], 1};
		ind = poly->vert[2].point[axis] > cam->fru_far[axis];
		io_points[ind][io_nb[ind]++] = (t_clip){poly->vert[2], 2};
		if (io_nb[1] == 1)
			__clipping_of_tri_1(to_clip, io_points, cam->fru_far, axis);
		else if (io_nb[1] == 2)
			__clipping_of_tri_2(to_clip, io_points, cam->fru_far, axis);
		if (io_nb[1] == 1 || io_nb[1] == 2 || io_nb[1] == 3)
			return (0);
	}
	return (1);
}

void	mesh_clipping(
			t_camera *const cam,
			t_vector *const to_clip,
			t_vector *const clipped)
{
	t_polygon	*poly;

	while (to_clip->size)
	{
		poly = ((t_polygon *)to_clip->data) + (--to_clip->size);
		if ((poly->vert[0].point[y] < cam->fru_near[y]
				|| poly->vert[0].point[y] > cam->fru_far[y]
				|| poly->vert[0].point[x] < cam->fru_near[x]
				|| poly->vert[0].point[x] > cam->fru_far[x])
			|| (poly->vert[1].point[y] < cam->fru_near[y]
				|| poly->vert[1].point[y] > cam->fru_far[y]
				|| poly->vert[1].point[x] < cam->fru_near[x]
				|| poly->vert[1].point[x] > cam->fru_far[x])
			|| (poly->vert[2].point[y] < cam->fru_near[y]
				|| poly->vert[2].point[y] > cam->fru_far[y]
				|| poly->vert[2].point[x] < cam->fru_near[x]
				|| poly->vert[2].point[x] > cam->fru_far[x]))
		{
			if (clip_poly_near(poly, to_clip, cam, 0))
				clip_poly_far(poly, to_clip, cam, 0);
		}
		else
			vector_addback(clipped, poly);
	}
}
