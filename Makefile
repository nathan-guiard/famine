# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nguiard <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 15:46:23 by nguiard           #+#    #+#              #
#    Updated: 2024/11/05 15:54:18 by nguiard          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC	=	$(addsuffix .c,		\
		$(addprefix src/,	\
			main			\
			profiling		\
			utils			\
			infection		\
			note_segment	\
		))

NAME	= famine

SHELL	= /bin/zsh

OBJ		= ${SRC:src/%.c=.obj/%.o} .obj/return.o

CC		= gcc

INCLUDE = -Iinclude/

CFLAGS	= -Wall -Werror -Wextra -pipe ${INCLUDE} -g3 #-fsanitize=address

all: debug

debug: CFLAGS += -DDEBUG
debug: fclean ${NAME}

prod: ${NAME}

.obj/%.o: src/%.c
	@${CC} ${CFLAGS} -c $< -o ${<:src/%.c=.obj/%.o}

.obj/%.o: src/%.s
	@nasm -f elf64 -o ${<:src/%.s=.obj/%.o} $<
	@chmod o+wr ${<:src/%.s=.obj/%.o}

${NAME}: ${OBJ}
	@echo "Compiling ${NAME}"
	@${CC} -nostartfiles ${OBJ} ${CFLAGS} -o ${NAME}
	@echo "${NAME} compiled"


clean:
	@rm -rf ${OBJ}


fclean:
	@rm -rf ${OBJ} ${NAME}


re: fclean all


.PHONY: all clean fclean re debug
