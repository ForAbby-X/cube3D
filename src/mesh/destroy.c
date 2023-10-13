/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:12:34 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/13 07:52:35 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

void	mesh_destroy(t_engine *const eng, t_mesh *const mesh)
{
	ft_destroy_sprite(eng, mesh->spr);
	vector_destroy(&mesh->polygons);
}
