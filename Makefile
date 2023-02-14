# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: doreshev <doreshev@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/26 23:28:55 by hoomen            #+#    #+#              #
#    Updated: 2023/02/09 17:00:20 by doreshev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	webserv

CXX			:=	c++
CXXFLAGS	:=	-Wall -Werror -Wextra -std=c++98

DEPFLAGS	=	-MT $@ -MMD -MP -MF $(DDIR)/$*.d

INCFLAGS = -I incl
VPATH		= src
SRC			=	CGI.cpp CLI.cpp Config.cpp ConfigParser.cpp DELETERequest.cpp \
					Env.cpp GETRequest.cpp main.cpp POSTRequest.cpp ResponseGenerator.cpp \
					Server.cpp Socket.cpp tools.cpp Session.cpp request_handler.cpp

ODIR		=	obj
OBJ			=	$(SRC:%.cpp=$(ODIR)/%.o)

DDIR		=	$(ODIR)/.deps
DEP			=	$(SRC:%.cpp=$(DDIR)/%.d)


.PHONY : all clean fclean re run bonus

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

$(ODIR)/%.o : %.cpp $(DDIR)/%.d | $(ODIR) $(DDIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCFLAGS) -c $< -o $@

$(ODIR) :
	mkdir $@

$(DDIR) :
	mkdir $@

all : $(NAME)

clean :
	$(RM) -r $(DDIR) $(ODIR)

fclean : clean
	$(RM) $(NAME)

re : fclean all

run : $(NAME)
	./$(NAME) config/vs_code.conf

bonus: all

$(DEP):
-include $(DEP)
