/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:53:15 by adinari           #+#    #+#             */
/*   Updated: 2022/11/01 20:13:05 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int i = 0;

void *routine(t_philo *philo)
{
	printf("Philosopher %d is thinking\n",philo->philo_id);
	pthread_mutex_lock(&philo->fork_mutex);//when philosopher 5 is eating he takes fork 1 and fork 5
	pthread_mutex_lock(&philo->next->fork_mutex);
	printf("Philosopher %d is eating\n ",philo->philo_id);
	sleep(3);
	pthread_mutex_unlock(&philo->fork_mutex);
	pthread_mutex_unlock(&philo->next->fork_mutex);
	printf("Philosopher %d Finished eating\n",philo->philo_id);
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

void	wait_threads(t_philo *philos)
{
	t_philo *tmp;

	tmp = philos;
	while (tmp)
	{
		int	k;
	
		k = pthread_join(philos->philo_thr, NULL);
		if(k!=0)
		{
			printf("\n Thread join failed \n");
			exit(1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
}

void	dstr_mutx(t_philo *philos)
{
	t_philo *tmp;

	tmp = philos;
	while (tmp)
	{
		int	k;
	
		k = pthread_mutex_destroy(&philos->fork_mutex);
		if(k!=0)
		{
			printf("\n Thread join failed \n");
			exit(1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
}

int main(int argc, char *argv[]) 
{
    int     n;
	t_philo *philos;
	t_philo *tmp;

	if (argc < 5 || argc > 6)//use is_digit on args
	{
		perror("Error : wrong number of arguments\n");
		exit(1);
	}
    n = atoi(argv[1]);
    philos = init_philosophers(argc, argv);
	display_list(philos);

	int k;
	tmp = philos;
	while (tmp)
	{
		k=pthread_mutex_init(&tmp->fork_mutex,NULL);
		if(k==-1)
		{
			printf("\nmutex initialized");
			exit(1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}

	tmp = philos;
	while (tmp)
	{
		k=pthread_create(&tmp->philo_thr,NULL,(void *)&routine, tmp);
		if(k!=0)
		{
			printf("\n Thread creation error \n");
			exit(1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	tmp = philos;
	while (tmp)
	{
		k=pthread_join(tmp->philo_thr,NULL);
		if(k!=0)
		{
			printf("\n Thread join failed \n");
			exit(1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	tmp = philos;
	while (tmp)
	{
		k=pthread_mutex_destroy(&tmp->fork_mutex);
		if(k!=0)
		{
			printf("\n Mutex Destroyed \n");
			exit(1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}

	system("leaks philosophers");
    return argc;
}