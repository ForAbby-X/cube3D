/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuller <vmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 22:20:24 by vmuller           #+#    #+#             */
/*   Updated: 2023/06/27 18:35:28 by vmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static inline void	__pars_clear(t_pars *const pars)
{
	t_length	index;

	index = 0;
	while (index < 6)
	{
		free(pars->elements[index]);
		index++;
	}
	while (ft_vector_size(pars->data))
		free(ft_vector_pop(pars->data));
}

int	pars_error(t_pars *const pars, char *const str)
{
	__pars_clear(pars);
	ft_putstr_fd("parsing error : ", 2);
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
		return (ft_putstr_fd("parsing error : wrong extension\n", 2),
			(t_map){0});
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return ((t_map){0});
	ft_memset(&pars, 0, sizeof(t_pars));
	pars.data = ft_vector_create(16);
	if (pars.data == NULL)
		return (get_next_line(-1), ft_vector_destroy(pars.data), (t_map){0});
	if (pars_elements(fd, &pars))
		return (get_next_line(-1), ft_vector_destroy(pars.data), (t_map){0});
	if (pars_map(fd, &pars))
		return (get_next_line(-1), ft_vector_destroy(pars.data), (t_map){0});
	get_next_line(-1);
	if (pars_to_map(eng, &pars, &map) || !is_map_closed(eng, &map))
		return (__pars_clear(&pars), ft_vector_destroy(pars.data), (t_map){0});
	__pars_clear(&pars);
	ft_vector_destroy(pars.data);
	return (map);
}
