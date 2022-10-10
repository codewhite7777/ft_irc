# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/17 07:01:17 by alee              #+#    #+#              #
#    Updated: 2022/09/26 15:44:20 by mgo              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror
CXXDEBUG = -g -fsanitize=address
CXX_VERSION = -std=c++98
RM = rm -f

NAME = ircserv

SRC_FILE	=	main.cpp \
				Server.cpp \
				Command.cpp \
				Protocol.cpp \
				Client.cpp \
				Channel.cpp \
				utils.cpp \
				Singleton.cpp

OBJ_FILE = $(SRC_FILE:.cpp=.o)
BONUS_FILE =

%.o	:	%.cpp
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(CXX_VERSION) -c $< -o $@

$(NAME): $(OBJ_FILE)
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(CXX_VERSION) $^ -o $@

all: $(NAME)

clean:
	$(RM) $(OBJ_FILE)

fclean: clean
	$(RM) $(NAME)

re:
	make fclean
	make all

.PHONY: all clean fclean re
