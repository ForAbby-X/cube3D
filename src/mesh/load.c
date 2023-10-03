/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:51:01 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/03 09:14:50 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"
#include "libft.h"

typedef enum e_vertex_type
{
	VERTEX,
	FACE,
	VTEXT,
	TEXTURE,
}	t_vertex_type;

typedef struct s_vertex_pars
{
	char const *const	str;
	size_t const		len;
	t_vertex_type const	type;
}	t_vertex_pars;

static t_vertex_pars const	g_vert_look[] = {
{"v ", 2, VERTEX},
{"vt ", 3, VTEXT},
{"f ", 2, FACE},
{"tex ", 4, TEXTURE},
{NULL, 0, 0}
};

t_vert	mesh_parse_get_v_uv(
						char **str,
						t_vector *const vertexs,
						t_vector *const uv_vec)
{
	t_vert	vert;

	vert = (t_vert){{0.f, 0.f, 0.f}, {0.5f, 0.5f}};
	while (**str == ' ')
		str++;
	if (**str < '1' || **str > '9')
		return (vert);
	vert.point = *((t_v3f *)vector_get(vertexs, ft_atoi(*str) - 1));
	while (**str >= '0' && **str <= '9')
		++(*str);
	if (**str == '\0' || **str == '\n')
		return (vert);
	if (**str != '/' || vector_size(uv_vec) == 0)
		return (mesh_parse_run_number(str), vert);
	++(*str);
	if (**str < '1' || **str > '9')
		return (mesh_parse_run_number(str), vert);
	vert.uv = *((t_v2f *)vector_get(uv_vec, ft_atoi(*str) - 1));
	vert.uv[x] = fminf(0.9999f, fmaxf(0.0001f, vert.uv[x]));
	vert.uv[y] = fminf(0.9999f, fmaxf(0.0001f, vert.uv[y]));
	return (mesh_parse_run_number(str), vert);
}

void	mesh_parse_run_number(char **str)
{
	while (**str == ' ')
		(*str)++;
	if (**str == '-')
		(*str)++;
	while (**str >= '0' && **str <= '9')
		(*str)++;
	while (**str == '.' || **str == '/')
		(*str)++;
	while (**str >= '0' && **str <= '9')
		(*str)++;
	while (**str == '.' || **str == '/')
		(*str)++;
	while (**str >= '0' && **str <= '9')
		(*str)++;
	if (**str == ' ')
		(*str)++;
}

static inline int	__parse_line(
						t_engine *const eng,
						char *str,
						t_mesh_pars const pars,
						t_mesh *const mesh)
{
	t_length	i;

	i = 0;
	while (g_vert_look[i].str
		&& ft_strncmp(g_vert_look[i].str, str, g_vert_look[i].len))
		i++;
	if (g_vert_look[i].str == NULL)
		return (0);
	str = str + g_vert_look[i].len;
	if (g_vert_look[i].type == VERTEX && mesh_parse_vertex(str, pars.vertexs))
		return (2);
	if (g_vert_look[i].type == VTEXT && mesh_parse_vertex_tex(str, pars.uvs))
		return (4);
	if (g_vert_look[i].type == TEXTURE && mesh_parse_textures(eng, str,
			&mesh->spr))
		return (5);
	if (g_vert_look[i].type == FACE && mesh_parse_face(str, pars.vertexs,
			pars.uvs, &mesh->polygons))
		return (3);
	return (0);
}

static inline int	__get_info(
			t_engine *const eng,
			int const fd,
			t_mesh *const mesh)
{
	char		*line;
	int			error;
	t_vector	vertex;
	t_vector	uv_vec;

	vertex = vector_create(sizeof(t_v3f));
	if (vertex.data == NULL)
		return (1);
	uv_vec = vector_create(sizeof(t_v2f));
	if (uv_vec.data == NULL)
		return (1);
	line = get_next_line(fd);
	error = 0;
	while (line && !error)
	{
		error = __parse_line(eng, line, (t_mesh_pars){
				NULL, &vertex, &uv_vec, {0}, {0}}, mesh);
		free(line);
		line = get_next_line(fd);
	}
	vector_destroy(&uv_vec);
	vector_destroy(&vertex);
	return (error);
}

t_mesh	mesh_load(t_engine *const eng, char *path)
{
	int const	fd = open(path, O_RDONLY);
	t_mesh		mesh;

	if (fd < 0)
		return (ft_putstr_fd("mesh error: could not load file \"", 2),
			ft_putstr_fd(path, 2), ft_putstr_fd("\"\n", 2), (t_mesh){0});
	if (ft_strncmp(ft_strrchr(path, '.'), ".obj", 5))
		return (close(fd), ft_putstr_fd("mesh error: wrong file format\n", 2),
			(t_mesh){0});
	mesh.polygons = vector_create(sizeof(t_polygon));
	if (mesh.polygons.data == NULL)
		return (close(fd), ft_putstr_fd("mesh error: mempry failure\n", 2),
			(t_mesh){0});
	mesh.spr = NULL;
	if (__get_info(eng, fd, &mesh))
		return (vector_destroy(&mesh.polygons), close(fd),
			ft_putstr_fd("ERROR: 4 loading mesh:\"", 2),
			ft_putstr_fd(path, 2), ft_putstr_fd("\"\n", 2), (t_mesh){0});
	if (mesh.spr == NULL)
	{
		mesh.spr = ft_sprite(eng, 1, 1);
		((u_int64_t *)mesh.spr->data)[0] = (u_int64_t){0x9003fc};
	}
	return (close(fd), mesh);
}
