# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alde-fre <alde-fre@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/25 09:39:09 by alde-fre          #+#    #+#              #
#    Updated: 2022/11/25 09:39:09 by alde-fre         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= cube3d

# directories
SRCDIR	=	./src
INCDIR	=	-I ./inc
OBJDIR	=	./obj

# src / includes / obj files
SRC		=	main.c \
			 \
			game/init.c \
			game/destroy.c \
			 \
			vec3/algorithm.c \
			vec3/rotation.c \
			vec3/convertion.c \
			 \
			parsing/map.c \
			parsing/elements.c \
			parsing/file.c \
			parsing/to_map.c \
			parsing/closed.c \
			parsing/utils.c \
			parsing/color_tint.c \
			 \
			cast_ray.c \
			 \
			map/manager.c \
			map/getter.c \
			map/setter.c \
			map/iterate.c \
			map/agrement/manager.c \
			map/agrement/doors.c \
			map/agrement/rooms.c \
			map/agrement/elevate.c \
			map/generation/all.c \
			map/generation/rooms.c \
			 \
			render/raycaster.c \
			render/reflection.c \
			render/shader.c \
			 \
			interface/container.c \
			interface/objects.c \
			interface/adder.c \
			interface/update.c \
			interface/display.c \
			 \
			inventory/hotbar.c \
			 \
			holding/holding.c \
			 \
			camera.c \
			 \
			menu/manager.c \
			menu/settings.c \
			 \
			minimap.c \
			 \
			mesh/load.c \
			mesh/destroy.c \
			mesh/parse.c \
			mesh/point.c \
			mesh/intersection.c \
			mesh/clipping.c \
			mesh/polygon.c \
			mesh/rasteriser.c \
			mesh/utils.c \
			mesh/transform.c \
			 \
			entity/manager.c \
			entity/collisions.c \
			entity/generic/manager.c \
			entity/player/manager.c \
			entity/player/controls.c \
			entity/fireball/manager.c \
			entity/mimic/manager.c \
			entity/door/manager.c \
			 \
			particle/manager.c \
			particle/generic.c \

INC		=	vec3.h \
			camera.h \
			map.h \
			cast_ray.h \
			raycaster.h \
			parsing.h \
			interface.h \
			aabb.h \
			menu.h \
			game.h \
			model.h \

OBJ		= $(addprefix $(OBJDIR)/,$(SRC:.c=.o))

DEPENDS := $(patsubst %.o,%.d,$(OBJ))


# compiler
CC		= cc
CFLAGS	= -MMD -MP -Wall -Wextra -Werror -g3

# engine library
ENGINE		= ./engine/
ENGINE_LIB	= $(addprefix $(ENGINE), libengine.a)
ENGINE_INC	= -I ./engine/inc
ENGINE_LNK	= -l Xext -l X11 -l Xfixes -L ./engine -l engine -l m

all: obj $(ENGINE_LIB) $(NAME)

raw: CFLAGS += -O0
raw: obj $(ENGINE_LIB) $(NAME)

fast: CFLAGS += -Ofast -mtune=corei7-avx
fast: obj $(ENGINE_LIB) $(NAME)

debug: CFLAGS += -gdwarf-4
debug: obj $(ENGINE_LIB) $(NAME)

obj:
	@mkdir -p $(OBJDIR)

.print:
	@> $@
	@echo "\e[1;36mCompiling...\e[0m"

$(NAME): $(OBJ)
	@echo "\e[1;35mLinking...\e[0m"
	@$(CC) -o $(NAME) $+ $(ENGINE_LNK)
	@echo "\e[1;32m➤" $@ "created succesfully !\e[0m"

$(OBJDIR)/%.o: $(SRCDIR)/%.c .print
	@echo "\e[0;36m ↳\e[0;36m" $<"\e[0m"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCDIR) $(ENGINE_INC) -c $< -o $@

temp:
	@echo "\e[1;36mCompiling...\e[0m";

$(ENGINE_LIB):
	@make -C $(ENGINE)

clean:
	rm -rf $(OBJDIR)
	make -C $(ENGINE) clean

fclean: clean
	rm -rf $(NAME)
	make -C $(ENGINE) fclean

re: fclean all

.PHONY: all clean fclean re

-include $(DEPENDS)
