# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adinari <adinari@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/29 16:24:13 by adinari           #+#    #+#              #
#    Updated: 2022/10/31 17:30:20 by adinari          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

FILES = pseudocode.c

OBJECTS = $(FILES:.c=.o)

# LIBFT = libft/libft.a

# @make -C ./libft

CC = gcc

FLAGS = -Wall -Werror -Wextra -pthread

all: $(NAME)

%.o:%.c
	@gcc $(FLAGS) -c $< -o $@

# LIBS = libft/libft.a

$(NAME): $(OBJECTS)
	@gcc $(OBJECTS) -o $(NAME)
# @cd libft && make
# @gcc $(OBJECTS) $(LIBS) -o $(NAME) 

clean:
	@rm -rf $(OBJECTS)
# @cd libft && make clean

fclean: clean
	@rm -rf $(NAME) *.o
# @cd libft && make fclean

re: fclean all