SRC	=	$(addsuffix .c,		\
		$(addprefix src/,	\
			main			\
		))

NAME	= famine

SHELL	= /bin/zsh

OBJ		= ${SRC:src/%.c=.obj/%.o}

CC		= gcc

INCLUDE = -Iinclude/

CFLAGS	=  -Wall -Werror -Wextra -pipe ${INCLUDE} -O3 -g3 #-fsanitize=address

all: ${NAME}


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


.PHONY: all clean fclean  re