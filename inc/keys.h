/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:42:06 by alde-fre          #+#    #+#             */
/*   Updated: 2023/11/14 19:04:17 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H
# ifdef AZERTY
#  define K_UP 'z'
#  define K_DOWN 's'
#  define K_LEFT 'q'
#  define K_RIGHT 'd'
# endif
# ifndef AZERTY
#  define K_UP 'w'
#  define K_DOWN 's'
#  define K_LEFT 'a'
#  define K_RIGHT 'd'
# endif
#endif
