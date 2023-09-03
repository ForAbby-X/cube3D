/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 01:40:17 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/03 16:19:55 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

int	ft_abs(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

void	draw_line(
		t_engine *const eng,
		t_v2i const f,
		t_v2i const s,
		t_color const color)
{
	t_v2i const	delta = {ft_abs(s[x] - f[x]), ft_abs(s[y] - f[y])};
	t_v2i const	sign = -(f < s) + (f >= s);
	int			error[2];
	t_v2i		cur;
	size_t		len;

	error[0] = delta[x] - delta[y];
	len = ft_max(delta[x], delta[y]);
	cur = f;
	while (len)
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
		len--;
	}
}

static inline unsigned int	__is_out(
		t_v3f const pos,
		t_camera *const cam)
{
	return (pos[y] < cam->fru_near[y] || pos[y] > cam->fru_far[y]
		|| pos[x] < cam->fru_near[x] || pos[x] > cam->fru_far[x]);
}

static inline t_v2i	__pix_min(t_v3f const a[3])
{
	return ((t_v2i){
		ft_min(a[0][x], ft_min(a[1][x], a[2][x])),
		ft_min(a[0][y], ft_min(a[1][y], a[2][y]))});
}

static inline t_v2i	__pix_max(t_v3f const a[3])
{
	return ((t_v2i){
		ft_max(a[0][x], ft_max(a[1][x], a[2][x])),
		ft_max(a[0][y], ft_max(a[1][y], a[2][y]))});
}

static inline float	__edge(t_v3f const a, t_v3f const b, t_v3f const c)
{
	return ((c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]));
}

static inline void	__polygon_put(
						t_engine *const eng,
						t_camera *const cam,
						t_v3f const pos[3],
						t_color const color)
{
	float const	area = __edge(pos[0], pos[1], pos[2]);
	t_v2i const	mm[2] = {__pix_min(pos), __pix_max(pos)};
	t_v3f		w;
	t_v3f		p;
	t_v2i		pix;
	float		depth;

	pix[y] = mm[0][y] - 1;
	while (++pix[y] < mm[1][y])
	{
		pix[x] = mm[0][x] - 1;
		while (++pix[x] < mm[1][x])
		{
			p = (t_v3f){pix[x] + .5f, pix[y] + .5f, 0.f};
			w = (t_v3f){
				__edge(pos[1], pos[2], p),
				__edge(pos[2], pos[0], p),
				__edge(pos[0], pos[1], p)} / area;
			if (w[0] < 0 || w[1] < 0 || w[2] < 0)
				continue ;
			depth = pos[0][z] * w[0] + pos[1][z] * w[1] + pos[2][z] * w[2];
			if (depth < camera_get_depth(cam, pix))
			{
				ft_draw(eng, pix, ft_color_inter(color, cam->fog_color,
					powf(1.0f - fmaxf(0.f, fminf(1.f, depth
						/ cam->fog_distance)), 2)));
				camera_set_depth(cam, pix, depth);
			}
		}
	}
}

t_v3f	inter_ray_box(t_v3f b1, t_v3f b2, t_v3f p1, t_v3f p2)
{
	t_v3f const	diff_r = p2 - p1;
	t_v3f const	t[2] = {(b2 - p1) / diff_r, (b1 - p1) / diff_r};
	t_v2f		tmm;

	enum e_minmax {min, max};
	tmm[min] = fminf(t[min][x], t[max][x]);
	tmm[max] = fmaxf(t[min][x], t[max][x]);
	tmm[min] = fmaxf(tmm[min], fminf(t[min][y], t[max][y]));
	tmm[max] = fminf(tmm[max], fmaxf(t[min][y], t[max][y]));
	tmm[min] = fmaxf(tmm[min], fminf(t[min][z], t[max][z]));
	tmm[max] = fminf(tmm[max], fmaxf(t[min][z], t[max][z]));
	if (tmm[min] > tmm[max])
		return (p2);
	if (tmm[min] < 0.f)
		return (p1 + diff_r * tmm[max]);
	return (p1 + diff_r * tmm[min]);
}

t_v3f	inter_ray_plane(t_v3f plane, t_xyz axis, t_v3f p1, t_v3f p2)
{
	t_v3f const	diff_r = p2 - p1;
	float const	t = (plane[axis] - p1[axis]) / diff_r[axis];

	if (t > 1.0f)
		return (p2);
	if (t < 0.0f)
		return (p1);
	return (p1 + diff_r * t);
}

static inline void	__clipping_of_tri_1(
						t_vector *const to_clip,
						t_v3f io_points[2][3],
						t_v3f const plane,
						t_xyz axis)
{
	t_v3f	corr[3];

	enum e_minmax {in, out};
	corr[0] = io_points[in][0];
	corr[1] = io_points[in][1];
	corr[2] = inter_ray_plane(plane, axis, io_points[in][0], io_points[out][0]);
	vector_addback(to_clip, corr);
	corr[0] = inter_ray_plane(plane, axis, io_points[in][1], io_points[out][0]);
	vector_addback(to_clip, corr);
}

static inline void	__clipping_of_tri_2(
						t_vector *const to_clip,
						t_v3f io_points[2][3],
						t_v3f const plane,
						t_xyz axis)
{
	t_v3f	corr[3];

	enum e_minmax {in, out};
	corr[0] = io_points[in][0];
	corr[1] = inter_ray_plane(plane, axis, io_points[in][0], io_points[out][0]);
	corr[2] = inter_ray_plane(plane, axis, io_points[in][0], io_points[out][1]);
	vector_addback(to_clip, corr);
}

static inline void	__clip_poly(
						t_v3f proj[3],
						t_vector *const to_clip,
						t_camera *const cam)
{
	t_v3f		io_points[2][3];
	t_length	inside_nb;
	t_length	outside_nb;
	t_xyz		axis;

	enum e_minmax {in, out};
	axis = x;
	while (axis <= y)
	{
		inside_nb = 0;
		outside_nb = 0;
		if (proj[0][axis] < cam->fru_near[axis])
			io_points[out][outside_nb++] = proj[0];
		else
			io_points[in][inside_nb++] = proj[0];
		if (proj[1][axis] < cam->fru_near[axis])
			io_points[out][outside_nb++] = proj[1];
		else
			io_points[in][inside_nb++] = proj[1];
		if (proj[2][axis] < cam->fru_near[axis])
			io_points[out][outside_nb++] = proj[2];
		else
			io_points[in][inside_nb++] = proj[2];
		if (outside_nb == 1)
			__clipping_of_tri_1(to_clip, io_points,
				cam->fru_near, axis);
		else if (outside_nb == 2)
			__clipping_of_tri_2(to_clip, io_points,
				cam->fru_near, axis);
		if (outside_nb == 1 || outside_nb == 2 || outside_nb == 3)
			return ;
		axis++;
	}
	axis = x;
	while (axis <= y)
	{
		inside_nb = 0;
		outside_nb = 0;
		if (proj[0][axis] > cam->fru_far[axis])
			io_points[out][outside_nb++] = proj[0];
		else
			io_points[in][inside_nb++] = proj[0];
		if (proj[1][axis] > cam->fru_far[axis])
			io_points[out][outside_nb++] = proj[1];
		else
			io_points[in][inside_nb++] = proj[1];
		if (proj[2][axis] > cam->fru_far[axis])
			io_points[out][outside_nb++] = proj[2];
		else
			io_points[in][inside_nb++] = proj[2];
		if (outside_nb == 1)
			__clipping_of_tri_1(to_clip, io_points,
				cam->fru_far, axis);
		else if (outside_nb == 2)
			__clipping_of_tri_2(to_clip, io_points,
				cam->fru_far, axis);
		if (outside_nb == 1 || outside_nb == 2 || outside_nb == 3)
			return ;
		axis++;
	}
}

static inline void	__clip_poly_depth(
						t_v3f proj[3],
						t_vector *const to_clip,
						t_camera *const cam)
{
	t_v3f		io_points[2][3];
	t_length	inside_nb;
	t_length	outside_nb;

	enum e_minmax {in, out};

	inside_nb = 0;
	outside_nb = 0;
	if (proj[0][z] < cam->fru_near[z])
		io_points[out][outside_nb++] = proj[0];
	else
		io_points[in][inside_nb++] = proj[0];
	if (proj[1][z] < cam->fru_near[z])
		io_points[out][outside_nb++] = proj[1];
	else
		io_points[in][inside_nb++] = proj[1];
	if (proj[2][z] < cam->fru_near[z])
		io_points[out][outside_nb++] = proj[2];
	else
		io_points[in][inside_nb++] = proj[2];
	if (outside_nb == 1)
		__clipping_of_tri_1(to_clip, io_points,
			cam->fru_near, z);
	else if (outside_nb == 2)
		__clipping_of_tri_2(to_clip, io_points,
			cam->fru_near, z);
	if (outside_nb == 1 || outside_nb == 2 || outside_nb == 3)
		return ;
	inside_nb = 0;
	outside_nb = 0;
	if (proj[0][z] > cam->fru_far[z])
		io_points[out][outside_nb++] = proj[0];
	else
		io_points[in][inside_nb++] = proj[0];
	if (proj[1][z] > cam->fru_far[z])
		io_points[out][outside_nb++] = proj[1];
	else
		io_points[in][inside_nb++] = proj[1];
	if (proj[2][z] > cam->fru_far[z])
		io_points[out][outside_nb++] = proj[2];
	else
		io_points[in][inside_nb++] = proj[2];
	if (outside_nb == 1)
		__clipping_of_tri_1(to_clip, io_points,
			cam->fru_far, z);
	else if (outside_nb == 2)
		__clipping_of_tri_2(to_clip, io_points,
			cam->fru_far, z);
	if (outside_nb == 1 || outside_nb == 2 || outside_nb == 3)
		return ;
	vector_addback(to_clip, proj);
}

static inline void	__clipping(
						t_camera *const cam,
						t_vector *const to_clip,
						t_vector *const clipped)
{
	t_polygon	poly;

	while (vector_size(to_clip))
	{
		poly = *((t_polygon *)vector_pop(to_clip));
		if (__is_out(poly.vert[0], cam) || __is_out(poly.vert[1], cam)
			|| __is_out(poly.vert[2], cam))
			__clip_poly(poly.vert, to_clip, cam);
		else
			vector_addback(clipped, &poly);
	}
}

static inline float	__is_culled(t_v3f proj[3], t_camera *const cam)
{
	t_v3f const	u = proj[1] - proj[0];
	t_v3f const	v = proj[2] - proj[1];
	t_v3f const	dir = v3fnorm((proj[0] + (u + v) / 2.f) - cam->pos, 1.f);
	t_v3f const	normal = v3fnorm(v3fcross(u, v), 1.f);

	return (v3fdot(dir, normal));
}

static inline t_v3f	__small_proj_point(t_v3f const point, t_camera *const cam)
{
	t_v3f	proj;
	t_v3f	res;

	proj = v3froty(point - cam->pos, -cam->rot[x]);
	proj = v3frotz(proj, -cam->rot[y]);
	res = (t_v3f){
		proj[z],
		-proj[y],
		proj[x]};
	return (res);
}

// / proj[x] * cam->screen_dist + cam->surface->size[x] / 2.f
// / proj[x] * cam->screen_dist + cam->surface->size[y] / 2.f

void	mesh_put(
			t_engine *const eng,
			t_camera *const cam,
			t_v3f const pos,
			t_mesh *const mesh)
{
	t_polygon	polygon;
	t_length	index;
	t_vector	clipped;
	t_vector	to_clip;
	float		rot;

	ft_eng_sel_spr(eng, cam->surface);
	clipped = vector_create(sizeof(t_polygon));
	if (clipped.data == NULL)
		return ;
	to_clip = vector_create(sizeof(t_polygon));
	if (to_clip.data == NULL)
		return ;
	index = 0;
	while (index < vector_size(&mesh->polygons))
	{
		polygon = *((t_polygon *)vector_get(&mesh->polygons, index));

		polygon.vert[0] *= .5f;
		polygon.vert[1] *= .5f;
		polygon.vert[2] *= .5f;
		polygon.vert[0] += pos;
		polygon.vert[1] += pos;
		polygon.vert[2] += pos;
		rot = __is_culled(polygon.vert, cam);
		if (rot < 0.f)
		{
			polygon.vert[0] = __small_proj_point(polygon.vert[0], cam);
			polygon.vert[1] = __small_proj_point(polygon.vert[1], cam);
			polygon.vert[2] = __small_proj_point(polygon.vert[2], cam);
			vector_clear(&to_clip);
			__clip_poly_depth(polygon.vert, &to_clip, cam);
			for (t_length i = 0; i < vector_size(&to_clip); i++)
			{
				t_polygon *poin = vector_get(&to_clip, i);
				poin->vert[0][x] = poin->vert[0][x] / poin->vert[0][z] * cam->screen_dist + cam->surface->size[x] / 2.f;
				poin->vert[0][y] = poin->vert[0][y] / poin->vert[0][z] * cam->screen_dist + cam->surface->size[y] / 2.f;
				poin->vert[1][x] = poin->vert[1][x] / poin->vert[1][z] * cam->screen_dist + cam->surface->size[x] / 2.f;
				poin->vert[1][y] = poin->vert[1][y] / poin->vert[1][z] * cam->screen_dist + cam->surface->size[y] / 2.f;
				poin->vert[2][x] = poin->vert[2][x] / poin->vert[2][z] * cam->screen_dist + cam->surface->size[x] / 2.f;
				poin->vert[2][y] = poin->vert[2][y] / poin->vert[2][z] * cam->screen_dist + cam->surface->size[y] / 2.f;
			}
			__clipping(cam, &to_clip, &clipped);
			while (vector_size(&clipped))
				__polygon_put(eng, cam, vector_pop(&clipped), ft_color_inter((t_color [5]){{0xFF0000}, {0xF0F000}, {0xFF00}, {0xF0F0}, {0xFF}}[index % 5], (t_color){0}, -rot));
		}
		index++;
	}
	vector_destroy(&to_clip);
	vector_destroy(&clipped);
	ft_eng_sel_spr(eng, NULL);
}
