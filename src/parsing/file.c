/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 22:20:24 by vmuller           #+#    #+#             */
/*   Updated: 2023/10/28 13:40:12 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static inline int	__pars_clear(t_pars *const pars)
{
	t_length	index;

	index = 0;
	while (index < 6)
	{
		free(pars->elements[index]);
		index++;
	}
	while (vector_size(&pars->data))
		free(*(char **)vector_pop(&pars->data));
	return (1);
}

int	pars_error(t_pars *const pars, char *const str)
{
	__pars_clear(pars);
	ft_putstr_fd("Error\nparsing error : ", 2);
	ft_putstr_fd(str, 2);
	write(2, (char [1]){'\n'}, 1);
	return (1);
}

t_map	pars_file(
	t_engine *const eng,
	char *const path)
{
	t_pars	pars;
	t_map	map;
	int		fd;

	if (!ft_strrchr(path, '.')
		|| ft_strncmp(ft_strrchr(path, '.'), ".cub", 5))
		return (ft_putstr_fd("Error\nparsing error : wrong extension\n", 2),
			(t_map){0});
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return ((t_map){0});
	pars = (t_pars){.data = vector_create(sizeof(char *))};
	if (pars.data.data == NULL)
		return (get_next_line(-1), vector_destroy(&pars.data), (t_map){0});
	if (pars_elements(fd, &pars) || pars_map(fd, &pars))
		return (get_next_line(-1), vector_destroy(&pars.data), (t_map){0});
	get_next_line(-1);
	if (pars_to_map(eng, &pars, &map)
		|| (!is_map_closed(&map) && __pars_clear(&pars)))
		return (vector_destroy(&pars.data), (t_map){0});
	__pars_clear(&pars);
	vector_destroy(&pars.data);
	return (map);
}
