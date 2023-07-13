/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 20:57:07 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/13 22:43:38 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

typedef struct s_lookup
{
	char *const		str;
	int const		index;
	t_length const	len;
}	t_lookup;

static t_lookup const	g_pars_look[] = {
{"NO", 0, 2},
{"EA", 2, 2},
{"SO", 1, 2},
{"WE", 3, 2},
{"C", 4, 1},
{"F", 5, 1},
{NULL, 0, 0}
};

static inline int	__line_get_elem(
	char *str,
	t_pars *const pars)
{
	t_length	index;
	char		*end;

	while (ft_strchr(" \t\v", *str))
		str++;
	index = 0;
	while (g_pars_look[index].str
		&& ft_strncmp(g_pars_look[index].str, str, g_pars_look[index].len))
		index++;
	if (g_pars_look[index].str == NULL
		|| !ft_strchr(" \t\v", str[g_pars_look[index].len]))
		return (*str != '\n' && pars_error(pars, "unrecognised token"));
	if (pars->elements[g_pars_look[index].index])
		return (pars_error(pars, "double element detected"));
	str += g_pars_look[index].len;
	while (ft_strchr(" \t\v", *str))
		str++;
	end = str;
	while (!ft_strchr(" \t\v\n\0", *end))
		end++;
	if (str == end)
		return (pars_error(pars, "no element detected"));
	*end = '\0';
	pars->elements[g_pars_look[index].index] = ft_strdup(str);
	return (0);
}

int	pars_elements(int const fd, t_pars *const pars)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (is_line_map_data(line))
		{
			pars->last_line = line;
			break ;
		}
		if (__line_get_elem(line, pars))
			return (free(line), 1);
		free(line);
		line = get_next_line(fd);
	}
	if (!(pars->elements[0] && pars->elements[1]
			&& pars->elements[2] && pars->elements[3]
			&& pars->elements[4] && pars->elements[5]))
		return (free(line), pars_error(pars, "missing one or more elements"));
	return (0);
}
