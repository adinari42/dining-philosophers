/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:59:26 by adinari           #+#    #+#             */
/*   Updated: 2022/11/07 20:55:29 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "libft/libft.h"
#include <sys/time.h>

typedef struct s_philo
{
	pthread_mutex_t fork_mutex;
	pthread_t		philo_thr;
	int				philo_id;
	int				philo_t_die;
	int				philo_t_eat;
	int				philo_t_sleep;
	int				remaining_eats;
	long			last_eat;
	struct timeval start;
	struct s_data 	*monitor;
	struct s_philo	*next;
	int				state;
	int				begin;
}				t_philo;

typedef struct s_data
{
	pthread_mutex_t print_mutex;
	pthread_mutex_t	creation_mutex;
	pthread_t		death_monit;
	int				total_p;
	int				isdead;
	int				iscreated;
}				t_data;

void	*routine(t_philo *philo);
t_philo	*ft_lstlast(t_philo *lst);
int		push(t_philo **thestack, int thevalue, char **argv, int argc);
int		fill_ll(int philo_id, char **argv, t_philo **philos, int argc);
int		init_junk(char **argv, int argc, t_philo **philos);
t_philo	*init_philosophers(int argc, char **argv);
/*thread_utils.c*/
int		init_forks(t_philo *philos);
int		start_dining(t_philo *philos);
void	*check_death(t_philo *philo);
int		death_monitor_thread(t_philo *philo);
int		init_philo_threads(t_philo *philos);
/*routine_utils.c*/
long long	ft_gettime(void);
void	ft_usleep(long int time_in_ms);
void	print(t_philo *philo);
void eating(t_philo *philo);
void	*routine(t_philo *philo);
/*extra.c*/
long	get_time_stamp(struct timeval *start_time);
t_philo	*ft_lstlast(t_philo *lst);
int join_threads(t_philo *philo);
int	destroy_mutexes(t_philo *philos);


#endif
