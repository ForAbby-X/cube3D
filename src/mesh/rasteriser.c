/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasteriser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 12:34:18 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/27 04:48:18 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

static inline t_v2i	__pix_min(t_vert const a[3])
{
	return ((t_v2i){
		ft_min(a[0].point[x], ft_min(a[1].point[x], a[2].point[x])),
		ft_min(a[0].point[y], ft_min(a[1].point[y], a[2].point[y]))});
}

static inline t_v2i	__pix_max(t_vert const a[3])
{
	return ((t_v2i){
		ft_max(a[0].point[x], ft_max(a[1].point[x], a[2].point[x])) + 1,
		ft_max(a[0].point[y], ft_max(a[1].point[y], a[2].point[y])) + 1});
}

static inline float	__edge(t_v3f const a, t_v3f const b, t_v3f const c)
{
	return ((c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]));
}

typedef struct s_rasterize
{
	t_v3f		w;
	t_sprite	*spr;
	t_v2i		pix;
}	t_rasterize;

static inline void	__rasterize_pix(
	t_engine *const eng,
	t_camera *const cam,
	t_polygon *const poly,
	t_rasterize const rast)
{
	t_v2f	uvb[3];
	t_v3f	pro;
	t_v3f	w;
	t_color	col;

	w = rast.w;
	uvb[0] = poly->vert[0].uv / poly->vert[0].point[z];
	uvb[1] = poly->vert[1].uv / poly->vert[1].point[z];
	uvb[2] = poly->vert[2].uv / poly->vert[2].point[z];
	pro[x] = uvb[0][x] * w[x] + uvb[1][x] * w[y] + uvb[2][x] * w[z];
	pro[y] = uvb[0][y] * w[x] + uvb[1][y] * w[y] + uvb[2][y] * w[z];
	pro[z] = 1.f / (1.f / poly->vert[0].point[z] * w[x]
			+ 1.f / poly->vert[1].point[z] * w[y]
			+ 1.f / poly->vert[2].point[z] * w[z]);
	if (pro[z] < camera_get_depth(cam, rast.pix))
	{
		col = ft_get_color(rast.spr, (t_v2i){
				pro[x] * rast.spr->size[x] * pro[z],
				pro[y] * rast.spr->size[y] * pro[z]});
		if (col.a == 0)
		{
			ft_draw(eng, rast.pix, col);
			camera_set_depth(cam, rast.pix, pro[z]);
		}
	}
}

void	polygon_put(
	t_engine *const eng,
	t_camera *const cam,
	t_polygon *const poly,
	t_sprite *const spr)
{
	t_v2i const	mm[2] = {__pix_min(poly->vert), __pix_max(poly->vert)};
	float const	area = __edge(poly->vert[0].point, poly->vert[1].point,
			poly->vert[2].point);
	t_v3f		w;
	t_v3f		p;
	t_v2i		pix;

	pix[y] = mm[0][y] - 1;
	while (++pix[y] < mm[1][y])
	{
		pix[x] = mm[0][x] - 1;
		while (++pix[x] < mm[1][x])
		{
			p = (t_v3f){pix[x] + 0.5f, pix[y] + 0.5f};
			w = (t_v3f){__edge(poly->vert[1].point, poly->vert[2].point, p),
				__edge(poly->vert[2].point, poly->vert[0].point, p),
				__edge(poly->vert[0].point, poly->vert[1].point, p)}
				/ area;
			if (w[0] < 0.f || w[1] < 0.f || w[2] < 0.f)
				continue ;
			__rasterize_pix(eng, cam, poly, (t_rasterize){w, spr, pix});
		}
	}
}
