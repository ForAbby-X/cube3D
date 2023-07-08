/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 09:38:43 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/08 15:44:33 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

int	ft_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	ft_swap(void **a, void **b)
{
	void	*c;

	c = *a;
	*a = *b;
	*b = c;
}

float	ft_rand(float min, float max)
{
	return ((max - min) * ((float)rand() / (float)RAND_MAX) + min);
}

void	_ft_itoa(int nb, char *tab)
{
	int	len;
	int	temp;
	int	neg;

	neg = nb < 0;
	nb = abs(nb);
	tab[0] = '-';
	temp = nb;
	len = neg + (nb == 0);
	while (temp)
	{
		len++;
		temp /= 10;
	}
	tab[len] = 0;
	while (len > neg)
	{
		tab[len - 1] = '0' + nb % 10;
		nb /= 10;
		len--;
	}
}
