/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:02:13 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/03 16:14:02 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_H
# define MODEL_H

# include "vec3.h"
# include "engine.h"
# include "camera.h"

typedef struct s_dsprite			t_dsprite;

typedef struct s_transform			t_transform;
typedef struct s_vert				t_vert;
typedef struct s_polygon			t_polygon;
typedef struct s_mesh				t_mesh;

struct s_transform
{
	t_v2f	rotation;
	t_v3f	resize;
	t_v3f	translation;
};

struct s_vert
{
	t_v3f	point;
	t_v2f	uv;
};

struct s_polygon
{
	t_vert		vert[3];
};

struct s_mesh
{
	t_vector	polygons;
	char		*name;
	t_sprite	*spr;
};

struct s_dsprite
{
	t_v2i		pos;
	t_sprite	*spr;
	float		depth;
};

typedef struct t_mesh_pars
{
	t_vector	*polygons;
	t_vector	*vertexs;
	t_vector	*uvs;
	t_vert		first_point;
	t_vert		last_point;
}	t_mesh_pars;

float	ft_atof(char *str);

int		mesh_parse_vertex(char *str, t_vector *const vertexs);
int		mesh_parse_face(
			char *str,
			t_vector *const vertexs,
			t_vector *const uv_vec,
			t_vector *const polygons);
int		mesh_parse_vertex_tex(char *str, t_vector *const uv_vec);
int		mesh_parse_textures(
			t_engine *const eng,
			char *str,
			t_sprite **const spr);

t_vert	mesh_parse_get_v_uv(
			char **str,
			t_vector *const vertexs,
			t_vector *const uv_vec);
void	mesh_parse_run_number(char **str);

t_mesh	mesh_load(t_engine *const eng, char *path);
void	mesh_destroy(t_engine *const eng, t_mesh *const mesh);

t_vert	inter_ray_pol(
			t_v3f const plane,
			t_xyz const axis,
			t_vert const p1,
			t_vert const p2);

int		clip_poly_near(
			t_polygon const *const poly,
			t_vector *const to_clip,
			t_camera *const cam,
			int const type);
int		clip_poly_far(
			t_polygon const *const poly,
			t_vector *const to_clip,
			t_camera *const cam,
			int const type);
void	mesh_clipping(
			t_camera *const cam,
			t_vector *const to_clip,
			t_vector *const clipped);

void	polygon_put(
			t_engine *const eng,
			t_camera *const cam,
			t_polygon *const poly,
			t_sprite *const spr);

void	vert_transform(
			t_vert *const vert,
			t_transform const tran,
			t_v3f const v_sc);

void	mesh_put(
			t_engine *const eng,
			t_camera *const cam,
			t_transform const tran,
			t_mesh *const mesh);

t_v3f	project_point(t_v3f const point, t_camera *const cam);

void	put_spr_scale(
			t_engine *const eng,
			t_camera *const cam,
			t_dsprite const spr,
			float const scale);
void	put_3d_point(
			t_engine *const eng,
			t_camera *const cam,
			t_v3f const pos,
			float const size);
void	put_3d_spr(
			t_engine *const eng,
			t_camera *const cam,
			t_sprite *const spr,
			t_v3f const pos);

#endif
