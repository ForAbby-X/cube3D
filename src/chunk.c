/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 05:35:17 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/09 08:32:08 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "entity/entity.h"

t_chunk	chunks_create(t_map *const map)
{
	t_chunk	chunks;
	t_v2i	pos;
	t_v2i	size;

	size = (t_v2i){map->size[x], map->size[y]} / CHUNK_SIZE + 1;
	chunks.data = malloc(sizeof(t_vector *) * size[x]);
	pos[x] = 0;
	while (pos[x] < size[x])
	{
		chunks.data[pos[x]] = malloc(sizeof(t_vector) * size[y]);
		pos[y] = 0;
		while (pos[y] < size[y])
		{
			chunks.data[pos[x]][pos[y]] = vector_create(sizeof(t_entity *));
			pos[y]++;
		}
		pos[x]++;
	}
	chunks.size = size;
	return (chunks);
}

void	chunks_destroy(t_chunk *const chunks)
{
	t_v2i	pos;

	pos[x] = 0;
	while (pos[x] < chunks->size[x])
	{
		pos[y] = 0;
		while (pos[y] < chunks->size[y])
		{
			vector_destroy(&chunks->data[pos[x]][pos[y]]);
			pos[y]++;
		}
		free(chunks->data[pos[x]]);
		pos[x]++;
	}
	free(chunks->data);
}

int	chunks_fill(t_chunk *const chunks, t_vector *const entities)
{
	t_entity	*entity;
	t_v3f		ent_pos;
	int			index;
	t_v2i		pos;

	pos[x] = -1;
	while (++pos[x] < chunks->size[x])
	{
		pos[y] = -1;
		while (++pos[y] < chunks->size[y])
			vector_clear(&chunks->data[pos[x]][pos[y]]);
	}
	index = -1;
	while (++index < (int)entities->size)
	{
		entity = vector_get(entities, index);
		ent_pos = entity->aabb.pos + entity->aabb.dim / 2.f;
		pos = (t_v2i){ent_pos[x], ent_pos[z]} / CHUNK_SIZE;
		if (pos[x] >= 0 && pos[x] < chunks->size[x]
			&& pos[y] >= 0 && pos[y] < chunks->size[y])
			vector_addback(&chunks->data[pos[x]][pos[y]], &entity);
	}
	return (0);
}

int	chunks_gather(t_chunk *const chunks, t_v3f const ent_pos, int const radius,
		t_vector *const out_ent)
{
	t_v2i	pos;
	t_v2i	chunk_pos;

	vector_clear(out_ent);
	pos[x] = -radius;
	while (pos[x] <= radius)
	{
		pos[y] = -radius;
		while (pos[y] <= radius)
		{
			chunk_pos = (t_v2i){ent_pos[x], ent_pos[z]} / CHUNK_SIZE + pos;
			if (chunk_pos[x] >= 0 && chunk_pos[x] < chunks->size[x]
				&& chunk_pos[y] >= 0 && chunk_pos[y] < chunks->size[y])
			{
				vector_insert_vector(out_ent,
					&chunks->data[chunk_pos[x]][chunk_pos[y]],
					out_ent->size);
			}
			++pos[y];
		}
		++pos[x];
	}
	return (0);
}
