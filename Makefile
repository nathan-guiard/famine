# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nguiard <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 15:46:23 by nguiard           #+#    #+#              #
#    Updated: 2024/11/25 12:25:44 by nguiard          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC	=	$(addsuffix .c,			\
		$(addprefix src/,		\
			main				\
			profiling			\
			utils				\
			infection			\
			note_segment		\
		))

NAME	= pestilence

SHELL	= /bin/zsh

OBJ		= ${SRC:src/%.c=.obj/%.o} .obj/start.o

CC		= gcc

INCLUDE = -Iinclude/

CFLAGS	= -Wall -Werror -Wextra -pipe ${INCLUDE} -g3 #-fsanitize=address

all: prod

debug: CFLAGS += -DDEBUG
debug: fclean ${NAME}

prod: ${NAME}

.obj/%.o: src/%.c
	@mkdir -p .obj
	@${CC} ${CFLAGS} -c $< -o ${<:src/%.c=.obj/%.o}

.obj/%.o: src/%.s
	@nasm -f elf64 -o ${<:src/%.s=.obj/%.o} $<
	@chmod o+wr ${<:src/%.s=.obj/%.o}

${NAME}: ${OBJ}
	@make -C exemple
	@echo "Compiling ${NAME}"
	@${CC} -nostartfiles ${OBJ} ${CFLAGS} -o ${NAME}
	@echo "${NAME} compiled"


clean:
	@rm -rf ${OBJ}


fclean:
	@rm -rf ${OBJ} ${NAME}


re: prod
	@make -C exemple


.PHONY: all clean fclean re debug
