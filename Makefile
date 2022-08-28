# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/17 07:01:17 by alee              #+#    #+#              #
#    Updated: 2022/08/28 18:32:48 by alee             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
CXX_VERSION = -std=c++98
RM = rm -f

NAME = ircserv

SRC_FILE = main.cpp server.cpp client.cpp channel.cpp
OBJ_FILE = $(SRC_FILE:.cpp=.o)
BONUS_FILE =

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ_FILE)
	$(CXX) $(CXXFLAGS) $(CXX_VERSION) $^ -o $@

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

