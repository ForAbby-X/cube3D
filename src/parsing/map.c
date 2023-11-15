/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 22:22:01 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/13 09:17:50 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_line_map_data(char *str)
{
	while (*str && *str == ' ')
		str++;
	return (ft_isdigit(*str));
}

static inline int	__line_get_map(char *str, t_pars *pars)
{
	char		*ptr;

	if (*str == '\n')
		return (pars_error(pars, "map data end on a carriage return"));
	ptr = str;
	while (*ptr && ft_strchr(" 01NSWE", *ptr))
	{
		if (*ptr && ft_strchr("NSWE", *ptr))
		{
			if (pars->rotation)
				return (pars_error(pars, "two or more spawn locations"));
			pars->rotation = *ptr;
			pars->spawn = (t_v3i){ptr - str, 1, vector_size(&pars->data)};
		}
		ptr++;
	}
	if (!ft_strchr(" 01NSWE\n\0", *ptr))
		return (pars_error(pars, "unknown map data"));
	if (ptr - str > pars->size[x])
		pars->size[x] = ptr - str;
	return (0);
}

int	pars_map(int const fd, t_pars *const pars)
{
	char	*line;
	char	*tmp;

	line = pars->last_line;
	while (line)
	{
		if (__line_get_map(line, pars))
			return (free(line), 1);
		tmp = ft_strdup(line);
		if (tmp == NULL)
			return (free(line), pars_error(pars, "map memory error"));
		if (vector_addback(&pars->data, &tmp) == NULL)
			return (free(line), free(tmp),
				pars_error(pars, "map memory error"));
		free(line);
		line = get_next_line(fd);
	}
	if (pars->rotation == 0)
		return (pars_error(pars, "no spawn location detected"));
	pars->size = (t_v3i){pars->size[x], 3, vector_size(&pars->data)};
	if (pars->size[x] < 3 || pars->size[z] < 3)
		return (pars_error(pars, "no room for anything"));
	return (0);
}
