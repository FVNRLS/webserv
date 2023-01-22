NAME = webserv

CC = c++

RM = rm -f

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC =	./src/main.cpp \
		./src/Config.cpp \
		./src/ConfigParser.cpp \
		./src/error_handler.cpp \
		./src/Socket.cpp \
		./src/Server.cpp \
		./src/CLI.cpp \
		./src/tools/split.cpp \
		./src/tools/trim.cpp

INCL =	./incl/

OBJ = ${SRC:.cpp=.o}
DEP = ${SRC:.cpp=.d}

.cpp.o:
	@${CC} ${FLAGS} -I ${INCL} -c $< -o ${<:.cpp=.o}

${NAME}: ${OBJ}
	@${CC} ${OBJ} -o ${NAME}
	@rm -rf obj && mkdir obj
	@mv ${OBJ} obj/

all: ${NAME}

clean:
	@rm -rf obj

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re