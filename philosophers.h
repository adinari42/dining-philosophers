/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:59:26 by adinari           #+#    #+#             */
/*   Updated: 2022/10/31 17:34:04 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


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