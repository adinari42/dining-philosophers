/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pseudocode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:50:28 by adinari           #+#    #+#             */
/*   Updated: 2022/11/01 16:27:50 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int i = 0;

void *routine(pthread_mutex_t *mutex)
{
    static int  count = 1;

    pthread_mutex_lock(mutex);
    printf("routine exec, count = %d\n", count);
    count++;
    pthread_mutex_unlock(mutex);
    return (NULL);
}

void	free_ll(t_philo *stack)
{
	t_philo	*tmp1;
	t_philo	*tmp;

	tmp = stack;
	tmp1 = NULL;
	while (tmp)
	{
		tmp1 = tmp;
		tmp = tmp->next;
		free(tmp1);
	}
	stack = NULL;
}

void	display_list(t_philo *philos)
{
	t_philo *tmp;

	tmp = philos;
	while (tmp)
	{
		printf("philo id = %d , die time = %d, eat time = %d, sleep time = %d , remain eats = %d\n",tmp->philo_id, tmp->philo_t_die, tmp->philo_t_eat, tmp->philo_t_sleep, tmp->remaining_eats);
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
}

int main(int argc, char *argv[]) 
{
    int     n;
	t_philo *philos;

	if (argc < 5 || argc > 6)//use is_digit on args
	{
		perror("Error : wrong number of arguments\n");
		exit(1);
	}
    n = atoi(argv[1]);
    philos = init_philosophers(argc, argv);
	display_list(philos);
	system("leaks philosophers");
    return argc;
}