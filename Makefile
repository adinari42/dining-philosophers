# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adinari <adinari@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/29 16:24:13 by adinari           #+#    #+#              #
#    Updated: 2022/11/07 00:37:17 by adinari          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

FILES = version3.c\
		init_utils.c

OBJECTS = $(FILES:.c=.o)

# LIBFT = libft/libft.a

# @make -C ./libft

CC = @gcc -fsanitize=thread

FLAGS = -Wall -Werror -Wextra -pthread 

all: $(NAME)

%.o:%.c
	gcc $(FLAGS) -c $< -o $@

LIBS = libft/libft.a

$(NAME): $(OBJECTS)
	@cd libft && make
	$(CC) $(OBJECTS) $(LIBS) -o $(NAME) 
	
# @gcc $(OBJECTS) -o $(NAME)


clean:
	@cd libft && make clean
	@rm -rf $(OBJECTS)


fclean: clean
	@cd libft && make fclean
	@rm -rf $(NAME) *.o

re: fclean all