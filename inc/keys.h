/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:42:06 by alde-fre          #+#    #+#             */
/*   Updated: 2023/07/17 10:07:01 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef AZERTY
# define K_UP XK_z
# define K_DOWN XK_s
# define K_LEFT XK_q
# define K_RIGHT XK_d
#endif
#ifndef AZERTY
# define K_UP XK_w
# define K_DOWN XK_s
# define K_LEFT XK_a
# define K_RIGHT XK_d
#endif
