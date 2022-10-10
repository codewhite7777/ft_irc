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

#privmsg.cpp
#quit.cpp
#kick.cpp
#invite.cpp
#join.cpp
#part.cpp
#Message.cpp
#mode.cpp

OBJ_FILE = $(SRC_FILE:.cpp=.o)
BONUS_FILE =

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ_FILE)
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(CXX_VERSION) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OBJ_FILE)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re:
	make fclean
	make all

