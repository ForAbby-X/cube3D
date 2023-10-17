/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 01:40:17 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/17 16:45:52 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

typedef struct s_poly_put
{
	t_vector *const	to_clip;
	t_vector *const	clipped;
	t_mesh *const	mesh;
}	t_poly_put;

static inline float	__is_culled(t_polygon *const poly, t_camera *const cam)
{
	t_v3f const	u = poly->vert[1].point - poly->vert[0].point;
	t_v3f const	v = poly->vert[2].point - poly->vert[1].point;
	t_v3f const	dir = (poly->vert[0].point + (u + v) / 2.f) - cam->pos;
	t_v3f const	normal = v3fcross(u, v);

	return (v3fdot(dir, normal));
}

static inline t_v3f	__small_proj_point(t_v3f const point, t_camera *const cam)
{
	t_v3f	proj;
	t_v3f	res;

	proj = v3froty(point - cam->pos, -cam->rot[x]);
	proj = v3frotz(proj, -cam->rot[y]);
	res = (t_v3f){proj[z], -proj[y], proj[x]};
	return (res);
}

static inline void	__apply_persp(t_camera *const cam, t_vector *const to_clip)
{
	t_polygon	*poin;
	t_length	i;

	i = 0;
	while (i < to_clip->size)
	{
		poin = vector_get(to_clip, i);
		poin->vert[0].point[x] = poin->vert[0].point[x] / poin->vert[0].point[z]
			* cam->screen_dist + cam->surface->size[x] / 2.f;
		poin->vert[0].point[y] = poin->vert[0].point[y] / poin->vert[0].point[z]
			* cam->screen_dist + cam->surface->size[y] / 2.f;
		poin->vert[1].point[x] = poin->vert[1].point[x] / poin->vert[1].point[z]
			* cam->screen_dist + cam->surface->size[x] / 2.f;
		poin->vert[1].point[y] = poin->vert[1].point[y] / poin->vert[1].point[z]
			* cam->screen_dist + cam->surface->size[y] / 2.f;
		poin->vert[2].point[x] = poin->vert[2].point[x] / poin->vert[2].point[z]
			* cam->screen_dist + cam->surface->size[x] / 2.f;
		poin->vert[2].point[y] = poin->vert[2].point[y] / poin->vert[2].point[z]
			* cam->screen_dist + cam->surface->size[y] / 2.f;
		i++;
	}
}

static inline void	__test(
			t_engine *const eng,
			t_camera *const cam,
			t_polygon poly,
			t_poly_put poly_put)
{
	t_polygon	*pol;

	poly.vert[0].point = __small_proj_point(poly.vert[0].point, cam);
	poly.vert[1].point = __small_proj_point(poly.vert[1].point, cam);
	poly.vert[2].point = __small_proj_point(poly.vert[2].point, cam);
	vector_clear(poly_put.to_clip);
	if (clip_poly_near(&poly, poly_put.to_clip, cam, 1)
		&& clip_poly_far(&poly, poly_put.to_clip, cam, 1))
		vector_addback(poly_put.to_clip, &poly);
	__apply_persp(cam, poly_put.to_clip);
	mesh_clipping(cam, poly_put.to_clip, poly_put.clipped);
	pol = vector_get(poly_put.clipped, poly_put.clipped->size - 1);
	while (poly_put.clipped->size)
	{
		polygon_put(eng, cam, pol, poly_put.mesh->spr);
		pol--;
		poly_put.clipped->size--;
	}
}

void	mesh_put(
			t_engine *const eng,
			t_camera *const cam,
			t_transform const tran,
			t_mesh *const mesh)
{
	t_polygon	polygon;
	t_length	index;
	t_vector	clipped;
	t_vector	to_clip;
	t_v3f const	v_sc = {sinf(tran.rotation[x]), cosf(tran.rotation[x]),
		sinf(tran.rotation[y]), cosf(tran.rotation[y])};

	ft_eng_sel_spr(eng, cam->surface);
	clipped = vector_create(sizeof(t_polygon));
	if (clipped.data == NULL)
		return ;
	to_clip = vector_create(sizeof(t_polygon));
	if (to_clip.data == NULL)
		return ;
	index = -1;
	while (++index < mesh->polygons.size)
	{
		polygon = *((t_polygon *)vector_get(&mesh->polygons, index));
		vert_transform(polygon.vert, tran, v_sc);
		if (__is_culled(&polygon, cam) < 0.f)
			__test(eng, cam, polygon, (t_poly_put){&to_clip, &clipped, mesh});
	}
	vector_destroy(&to_clip);
	vector_destroy(&clipped);
	ft_eng_sel_spr(eng, NULL);
}
