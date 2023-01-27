NAME = webserv

CC = c++

RM = rm -f

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC =	./src/main.cpp \
		./src/Config.cpp \
		./src/ConfigParser.cpp \
		./src/Socket.cpp \
		./src/Server.cpp \
		./src/CLI.cpp \
		./src/ResponseGenerator.cpp \
		./src/tools.cpp \


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

run: $(NAME)
	./webserv config/vs_code.conf

.PHONY: all clean fclean re