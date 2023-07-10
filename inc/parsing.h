/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 20:57:21 by vmuller           #+#    #+#             */
/*   Updated: 2023/07/10 17:53:19 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "map.h"
# include "engine.h"

typedef struct s_pars
{
	char		*elements[6];
	t_vector	data;
	t_v3i		size;
	t_v3i		spawn;
	char		rotation;
	char		*last_line;
}	t_pars;

int			pars_error(t_pars *const pars, char *const str);

int			ft_ato_u8(char const *const input, int *err);

t_sprite	*load_tint_sprite(
				t_engine *eng,
				char *const path,
				t_color const tint);

int			is_line_map_data(char *str);
int			pars_map(int const fd, t_pars *const pars);

int			pars_elements(int const fd, t_pars *const pars);

t_map		pars_file(t_engine *const eng, char *const path);

int			pars_to_map(
				t_engine *const eng,
				t_pars *const pars,
				t_map *const map);

int			is_map_closed(t_engine *const eng, t_map *const map);

#endif
