/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 05:33:45 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/09 08:33:05 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_H
# define CHUNK_H

# define CHUNK_SIZE (1)

# include "map.h"

typedef struct s_chunk	t_chunk;

t_chunk	chunks_create(t_map *const map);
void	chunks_destroy(t_chunk *const chunks);

int		chunks_fill(t_chunk *const chunks, t_vector *const entities);
int		chunks_gather(t_chunk *const chunks, t_v3f const pos, int const radius,
			t_vector *const entities);

struct s_chunk
{
	t_vector	**data;
	t_v2i		size;
};

#endif
