/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:51:01 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/24 03:50:18 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"
#include "libft.h"

static inline float	__atof(char *str)
{
	int		sign;
	long	number;
	size_t	point;
	int		flag;

	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	number = 0;
	point = 1;
	flag = 0;
	while ((*str >= '0' && *str <= '9') || *str >= '.')
	{
		if (*str == '.')
			flag = 1;
		else
			number = number * 10 + (*str - '0');
		if (*str != '.' && flag != 0)
			point *= 10;
		str++;
	}
	return ((float)number / point * (float)sign);
}	

static inline void	__run_number(char **str)
{
	if (**str == '-')
		(*str)++;
	while (**str >= '0' && **str <= '9')
		(*str)++;
	if (**str == '.')
		(*str)++;
	while (**str >= '0' && **str <= '9')
		(*str)++;
	while (**str == ' ')
		(*str)++;
}

static inline t_v3f	__parse_line(char *str, int *const err)
{
	t_v3f		vertex;

	while (*str == ' ')
		str++;
	if (ft_strncmp(str, "v ", 2))
		return (*err = 1, (t_v3f){0});
	str = ft_strchr(str, 'v') + 1;
	while (*str == ' ')
		str++;
	if ((*str < '0' || *str > '9') && *str != '-')
		return (*err = 2, (t_v3f){0});
	vertex[x] = __atof(str);
	__run_number(&str);
	if ((*str < '0' || *str > '9') && *str != '-')
		return (*err = 2, (t_v3f){0});
	vertex[y] = __atof(str);
	__run_number(&str);
	if ((*str < '0' || *str > '9') && *str != '-')
		return (*err = 3, (t_v3f){0});
	vertex[z] = __atof(str);
	return (*err = 0, vertex);
}

static inline int	__get_info(int const fd, t_vector *const polygons)
{
	char		*line;
	int			error;
	t_polygon	polygon;
	int			counter;

	line = get_next_line(fd);
	error = 0;
	counter = 0;
	while (line && (error == 0 || error == 1))
	{
		polygon.points[counter] = __parse_line(line, &error);
		printf("VERTEX: [%f, %f, %f]\n",
				polygon.points[counter][x], polygon.points[counter][y], polygon.points[counter][z]);
		if (error == 0)
		{
			counter++;
			if (counter == 3)
			{
				counter = 0;
				vector_addback(polygons, &polygon);
			}
		}
		free(line);
		line = get_next_line(fd);
	}
	printf("ERROR : %d %d %d\n", error < 0, error > 1, counter > 0);
	return (error < 0 || error > 1 || counter > 0);
}

t_mesh	mesh_load(char *path)
{
	int const	fd = open(path, O_RDONLY);
	t_mesh		mesh;

	if (fd < 0)
		return (ft_putstr_fd("ERROR: 1 loading mesh:\"", 2),
			ft_putstr_fd(path, 2), ft_putstr_fd("\"\n", 2), (t_mesh){0});
	if (ft_strncmp(ft_strrchr(path, '.'), ".obj", 5))
		return (ft_putstr_fd("ERROR: 2 loading mesh:\"", 2),
			ft_putstr_fd(path, 2), ft_putstr_fd("\"\n", 2), (t_mesh){0});
	mesh.polygons = vector_create(sizeof(t_polygon));
	if (mesh.polygons.data == NULL)
		return (close(fd), ft_putstr_fd("ERROR: 3 loading mesh:\"", 2),
			ft_putstr_fd(path, 2), ft_putstr_fd("\"\n", 2), (t_mesh){0});
	if (__get_info(fd, &mesh.polygons))
		return (close(fd), ft_putstr_fd("ERROR: 4 loading mesh:\"", 2),
			ft_putstr_fd(path, 2), ft_putstr_fd("\"\n", 2), (t_mesh){0});
	close(fd);
	return (mesh);
}
