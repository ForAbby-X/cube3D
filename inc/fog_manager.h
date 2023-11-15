/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fog_manager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuller <vmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:18:37 by vmuller           #+#    #+#             */
/*   Updated: 2023/11/11 00:16:59 by vmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FOG_MANAGER_H
# define FOG_MANAGER_H

# include "engine.h"

typedef struct s_fog		t_fog;

struct s_fog
{
	float	start;
	float	time;
	float	end;
};

#endif 