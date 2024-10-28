# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nguiard <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 15:46:23 by nguiard           #+#    #+#              #
#    Updated: 2024/10/28 14:18:06 by nguiard          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC	=	$(addsuffix .c,		\
		$(addprefix src/,	\
			main			\
			valid_file		\
			file_operations	\
			print_elf		\
		))

NAME	= famine

SHELL	= /bin/zsh

OBJ		= ${SRC:src/%.c=.obj/%.o}

CC		= gcc

INCLUDE = -Iinclude/

CFLAGS	= -Wall -Werror -Wextra -pipe ${INCLUDE} -O3 -g3 #-fsanitize=address

all: debug

debug: CFLAGS += -DDEBUG
debug: fclean ${NAME}

prod: ${NAME}

.obj/%.o: src/%.c
	@${CC} ${CFLAGS} -c $< -o ${<:src/%.c=.obj/%.o}


${NAME}: ${OBJ}
	@echo "Compiling ${NAME}"
	@${CC} ${OBJ} ${CFLAGS} -o ${NAME}
	@echo "${NAME} compiled"


clean:
	@rm -rf ${OBJ}


fclean:
	@rm -rf ${OBJ} ${NAME}


re: fclean all


.PHONY: all clean fclean re debug
