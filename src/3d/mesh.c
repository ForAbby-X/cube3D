/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:51:01 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/03 15:41:01 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"
#include "libft.h"

typedef enum e_vertex_type
{
	VERTEX,
	FACE
}	t_vertex_type;

typedef struct s_vertex_pars
{
	char const *const	str;
	size_t const		len;
	t_vertex_type const	type;
}	t_vertex_pars;

static t_vertex_pars const g_vert_look[] = {
{"v ", 2, VERTEX},
{"f ", 2, FACE},
{NULL, 0, 0}
};

static inline void	__run_number(char **str)
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

static inline int	__parse_vertex(
						char *str,
						t_vector *const vertexs)
{
	t_v3f	vertex;

	while (*str == ' ')
		str++;
	if ((*str < '0' || *str > '9') && *str != '-')
		return (1);
	vertex[x] = ft_atof(str);
	__run_number(&str);
	if ((*str < '0' || *str > '9') && *str != '-')
		return (1);
	vertex[y] = ft_atof(str);
	__run_number(&str);
	if ((*str < '0' || *str > '9') && *str != '-')
		return (1);
	vertex[z] = ft_atof(str);
	__run_number(&str);
	if (vector_addback(vertexs, &vertex) == NULL)
		return (1);
	return (0);
}

typedef struct s_pars_face
{
	t_v3f	first_point;
	t_v3f	last_point;
}	t_pars_face;

static inline int	__parse_face_complex(
						char *str,
						t_vector *const vertexs,
						t_vector *const polygons,
						t_pars_face comp)
{
	t_polygon	poly;

	poly.vert[0] = comp.first_point;
	poly.vert[1] = comp.last_point;
	while (*str >= '0' && *str <= '9')
	{
		poly.vert[2] = *((t_v3f *)vector_get(vertexs, ft_atoi(str) - 1));
		__run_number(&str);
		if (vector_addback(polygons, &poly) == NULL)
			return (1);
		poly.vert[1] = poly.vert[2];
		__run_number(&str);
	}
	return (0);
}

static inline int	__parse_face(
						char *str,
						t_vector *const vertexs,
						t_vector *const polygons)
{
	t_polygon	poly;
	int			num;

	while (*str == ' ')
		str++;
	num = ft_atoi(str);
	if (*str < '1' || *str > '9')
		return (1);
	poly.vert[0] = *((t_v3f *)vector_get(vertexs, num - 1));
	__run_number(&str);
	num = ft_atoi(str);
	if (*str < '1' || *str > '9')
		return (1);
	poly.vert[1] = *((t_v3f *)vector_get(vertexs, num - 1));
	__run_number(&str);
	num = ft_atoi(str);
	if (*str < '1' || *str > '9')
		return (1);
	poly.vert[2] = *((t_v3f *)vector_get(vertexs, num - 1));
	__run_number(&str);
	if (vector_addback(polygons, &poly) == NULL)
		return (1);
	if (*str >= '0' && *str <= '9')
		return (__parse_face_complex(str, vertexs, polygons,
				(t_pars_face){poly.vert[0], poly.vert[2]}));
	return (0);
}

static inline int	__parse_line(
						char *str,
						t_vector *const vertex,
						t_vector *const polygons)
{
	t_length	i;

	i = 0;
	while (g_vert_look[i].str
		&& ft_strncmp(g_vert_look[i].str, str, g_vert_look[i].len))
		i++;
	if (g_vert_look[i].str == NULL)
		return (0);
	str = str + g_vert_look[i].len;
	if (g_vert_look[i].type == VERTEX && __parse_vertex(str, vertex))
		return (2);
	if (g_vert_look[i].type == FACE && __parse_face(str, vertex, polygons))
		return (3);
	return (0);
}

static inline int	__get_info(int const fd, t_vector *const polygons)
{
	char		*line;
	int			error;
	t_vector	vertex;

	vertex = vector_create(sizeof(t_v3f));
	if (vertex.data == NULL)
		return (1);
	line = get_next_line(fd);
	error = 0;
	while (line && !error)
	{
		error = __parse_line(line, &vertex, polygons);
		free(line);
		line = get_next_line(fd);
	}
	printf("ERROR [%d]\n", error);
	vector_destroy(&vertex);
	return (error);
}

t_mesh	mesh_load(char *path)
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
	if (__get_info(fd, &mesh.polygons))
		return (vector_destroy(&mesh.polygons), close(fd),
			ft_putstr_fd("ERROR: 4 loading mesh:\"", 2),
			ft_putstr_fd(path, 2), ft_putstr_fd("\"\n", 2), (t_mesh){0});
	close(fd);
	return (mesh);
}
