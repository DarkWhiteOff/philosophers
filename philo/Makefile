# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zamgar <zamgar@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/15 14:24:43 by zamgar            #+#    #+#              #
#    Updated: 2025/02/06 19:20:46 by zamgar           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC = main.c utils.c init.c threads.c check.c others.c

OBJS := $(SRC:%.c=%.o)

CC = gcc
FLAGS = -Wall -Wextra -Werror -O3 -pthread -I ./includes

# -fsanitize=thread

.c.o:
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
