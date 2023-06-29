/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:28:57 by alde-fre          #+#    #+#             */
/*   Updated: 2023/06/29 11:41:49 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned char	ft_ato_u8(char const *const input, int *err)
{
	unsigned int	i;
	unsigned char	ret;

	i = 0;
	if (input[i] == '-')
		return (*err = 1, 0);
	ret = 0;
	while (input[i] >= '0' && input[i] <= '9')
	{
		if ((unsigned char)(ret * 10 + (input[i] - '0')) > ret * 10 + 9
			|| (unsigned char)(ret * 10 + (input[i] - '0')) < ret * 10)
			return (*err = 1, 0);
		ret = ret * 10 + (input[i] - '0');
		i++;
	}
	return (*err = 0, ret);
}