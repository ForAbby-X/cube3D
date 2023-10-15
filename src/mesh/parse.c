/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:52:20 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/15 11:30:04 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

typedef enum e_vertex_type
{
	VERTEX,
	FACE,
	VTEXT,
	TEXTURE,
}	t_vertex_type;

int	mesh_parse_vertex(char *str, t_vector *const vertexs)
{
	t_v3f	vertex;

	while (*str == ' ')
		str++;
	if ((*str < '0' || *str > '9') && *str != '-')
		return (1);
	vertex[x] = ft_atof(str);
	mesh_parse_run_number(&str);
	while (*str == ' ')
		str++;
	if ((*str < '0' || *str > '9') && *str != '-')
		return (1);
	vertex[y] = ft_atof(str);
	mesh_parse_run_number(&str);
	while (*str == ' ')
		str++;
	if ((*str < '0' || *str > '9') && *str != '-')
		return (1);
	vertex[z] = ft_atof(str);
	return (vector_addback(vertexs, &vertex) == NULL);
}

static inline int	__parse_face_complex(char **str, t_mesh_pars pars)
{
	t_polygon	poly;

	poly.vert[0] = pars.first_point;
	poly.vert[1] = pars.last_point;
	printf("START complex \n\t[%f, %f, %f]\n\t[%f, %f, %f]\n\t[%f, %f, %f]\n", poly.vert[0].point[x], poly.vert[0].point[y], poly.vert[0].point[z], poly.vert[1].point[x], poly.vert[1].point[y], poly.vert[1].point[z], poly.vert[2].point[x], poly.vert[2].point[y], poly.vert[2].point[z]);
	while (**str >= '1' && **str <= '9')
	{
		poly.vert[2] = mesh_parse_get_v_uv(str, pars.vertexs, pars.uvs);
		printf("DURING complex \n\t[%f, %f, %f]\n\t[%f, %f, %f]\n\t[%f, %f, %f]\n", poly.vert[0].point[x], poly.vert[0].point[y], poly.vert[0].point[z], poly.vert[1].point[x], poly.vert[1].point[y], poly.vert[1].point[z], poly.vert[2].point[x], poly.vert[2].point[y], poly.vert[2].point[z]);
		if (vector_addback(pars.polygons, &poly) == NULL)
			return (1);
		poly.vert[1] = poly.vert[2];
	}
	return (0);
}

int	mesh_parse_face(
		char *str,
		t_vector *const vertexs,
		t_vector *const uv_vec,
		t_vector *const polygons)
{
	t_polygon	poly;

	while (*str == ' ')
		str++;
	if (*str < '1' || *str > '9')
		return (1);
	poly.vert[0] = mesh_parse_get_v_uv(&str, vertexs, uv_vec);
	poly.vert[1] = mesh_parse_get_v_uv(&str, vertexs, uv_vec);
	poly.vert[2] = mesh_parse_get_v_uv(&str, vertexs, uv_vec);
	if (vector_addback(polygons, &poly) == NULL)
		return (1);
	while (*str == ' ')
		str++;
	if (*str >= '0' && *str <= '9')
		return (__parse_face_complex(&str, (t_mesh_pars){polygons, vertexs,
				uv_vec, poly.vert[0], poly.vert[2]}));
	return (0);
}

int	mesh_parse_vertex_tex(char *str, t_vector *const uv_vec)
{
	t_v2f	uv;

	while (*str == ' ')
		str++;
	if ((*str < '0' || *str > '9') && *str != '-')
		return (1);
	uv[x] = ft_atof(str);
	mesh_parse_run_number(&str);
	while (*str == ' ')
		str++;
	if ((*str < '0' || *str > '9') && *str != '-')
		return (1);
	uv[y] = 1.f - ft_atof(str);
	return (vector_addback(uv_vec, &uv) == NULL);
}

int	mesh_parse_textures(t_engine *const eng, char *str, t_sprite **const spr)
{
	char	*temp;

	while (*str == ' ')
		str++;
	if (*str != '"')
		return (1);
	str++;
	temp = ft_strchr(str, '"');
	if (temp == NULL)
		return (1);
	*temp = '\0';
	*spr = ft_sprite_p(eng, str);
	if (*spr == NULL)
		return (1);
	return (0);
}
