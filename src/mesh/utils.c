/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:20:53 by alde-fre          #+#    #+#             */
/*   Updated: 2023/09/17 11:56:06 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.h"

float	ft_atof(char *str)
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
