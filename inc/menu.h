/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:34:19 by alde-fre          #+#    #+#             */
/*   Updated: 2023/10/26 13:46:07 by alde-fre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_H
# define MENU_H

# include "interface.h"

typedef long			t_menu_id;
typedef struct s_menu	t_menu;

struct s_menu
{
	t_vector	data;
	t_menu_id	selected;
};

t_menu	menu_create(void);
void	menu_destroy(t_menu *const menu);

t_gui	*menu_add(t_menu *const menu, t_gui *const gui);

void	menu_update(t_engine *const eng, t_menu *const menu);
void	menu_display(t_engine *const eng, t_menu *const menu);

int		menu_settings_create(t_engine *const eng, t_data *const data);
int		menu_main_create(t_engine *const eng, t_data *const data);

#endif
