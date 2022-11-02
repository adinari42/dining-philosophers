# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adinari <adinari@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/29 16:24:13 by adinari           #+#    #+#              #
#    Updated: 2022/11/02 16:44:04 by adinari          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

FILES = version2.c\
		init_utils.c

OBJECTS = $(FILES:.c=.o)

# LIBFT = libft/libft.a

# @make -C ./libft

CC = gcc

FLAGS = -Wall -Werror -Wextra -pthread  -g -fsanitize=thread

all: $(NAME)

%.o:%.c
	@gcc $(FLAGS) -c $< -o $@

LIBS = libft/libft.a

$(NAME): $(OBJECTS)
	@cd libft && make
	@gcc $(OBJECTS)  -g -fsanitize=thread $(LIBS) -o $(NAME) 
	
# @gcc $(OBJECTS) -o $(NAME)


clean:
	@cd libft && make clean
	@rm -rf $(OBJECTS)


fclean: clean
	@cd libft && make fclean
	@rm -rf $(NAME) *.o

re: fclean all