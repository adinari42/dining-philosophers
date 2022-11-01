/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:59:26 by adinari           #+#    #+#             */
/*   Updated: 2022/11/01 19:43:34 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "libft/libft.h"


typedef struct s_philo
{
	pthread_mutex_t fork_mutex;
	pthread_t		philo_thr;
	int				philo_id;
	int				philo_t_die;
	int				philo_t_eat;
	int				philo_t_sleep;
	int				remaining_eats;
	struct t_data 	*data;
	struct s_philo	*next;
}				t_philo;

typedef struct s_data
{
	pthread_mutex_t monitor_mutex;
	int				total_philo;
	int				philo_t_die;
	int				philo_t_eat;
	int				philo_t_sleep;
	int				number_of_eats;
}				t_data;

void	*routine(t_philo *philo);
t_philo	*ft_lstlast(t_philo *lst);
int		push(t_philo **thestack, int thevalue, char **argv, int argc);
int		fill_ll(int philo_id, char **argv, t_philo **philos, int argc);
int		init_junk(char **argv, int argc, t_philo **philos);
t_philo	*init_philosophers(int argc, char **argv);

#endif