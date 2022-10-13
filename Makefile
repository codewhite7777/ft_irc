# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/12 11:20:17 by mgo               #+#    #+#              #
#    Updated: 2022/10/12 11:20:18 by mgo              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror
CXXDEBUG = -g -fsanitize=address
CXX_VERSION = -pedantic -std=c++98
RM = rm -f

NAME = ircserv

SRC_FILE	=	main.cpp \
				Server.cpp \
				Command.cpp \
				Protocol.cpp \
				Client.cpp \
				Channel.cpp \
				utils.cpp \
				Singleton.cpp \
				Chatbot.cpp

OBJ_FILE = $(SRC_FILE:.cpp=.o)

%.o	:	%.cpp
	$(CXX) $(CXX_VERSION) $(CXXFLAGS) $(CXXDEBUG) -c $< -o $@

$(NAME): $(OBJ_FILE)
	$(CXX) $(CXX_VERSION) $(CXXFLAGS) $(CXXDEBUG) $^ -o $@

all: $(NAME)

clean:
	$(RM) $(OBJ_FILE)

fclean: clean
	$(RM) $(NAME)

re:
	make fclean
	make all

.PHONY: all clean fclean re
