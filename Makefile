# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adinari <adinari@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/29 16:24:13 by adinari           #+#    #+#              #
#    Updated: 2022/11/08 20:30:52 by adinari          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

FILES = philosophers.c\
		init_utils.c\
		thread_utils.c\
		extra.c\
		routine_utils.c

OBJECTS = $(FILES:.c=.o)

# LIBFT = libft/libft.a

# @make -C ./libft

CC = @gcc #-fsanitize=thread

FLAGS = -Wall -Werror -Wextra -pthread 

all: $(NAME)

%.o:%.c
	gcc $(FLAGS) -c $< -o $@

LIBS = libft/libft.a

$(NAME): $(OBJECTS)
	@cd libft && make
	$(CC) $(OBJECTS) $(LIBS) -o $(NAME) 

clean:
	@cd libft && make clean
	@rm -rf $(OBJECTS)


fclean: clean
	@cd libft && make fclean
	@rm -rf $(NAME) *.o

re: fclean all