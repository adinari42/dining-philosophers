/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:53:15 by adinari           #+#    #+#             */
/*   Updated: 2022/11/03 16:39:48 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"



long	get_time_stamp(struct timeval *start_time)
{
	struct timeval	timestamp;
	long			time;

	gettimeofday(&timestamp, NULL);
	timestamp.tv_sec -= start_time->tv_sec;
	timestamp.tv_usec -= start_time->tv_usec;
	if (timestamp.tv_usec < 0)
	{
		timestamp.tv_sec--;
		timestamp.tv_usec += 1000000;
	}
	time = timestamp.tv_sec * 1000;
	time += timestamp.tv_usec / 1000;
	return (time);
}


// void mutex_printf(char *str,pthread_mutex_t *mutex)
// {
// 	pthread_mutex_lock(mutex);
// 	printf("%s",str);
// 	pthread_mutex_unlock(mutex);
// }
int g_int = 0;
struct timeval *g_start = NULL;
int death_flag = 0;
void eating(t_philo *philo)
{
// printf  to  get forks and 	// check here for death
	pthread_mutex_lock(&philo->fork_mutex);//when philosopher 5 is eating he takes fork 1 and fork 5 // prevent deadlock
// printf  to  get forks and 	// check here for death
	pthread_mutex_lock(&philo->next->fork_mutex);// prevent deadlock
	printf("%li ms Philosopher %d is eating\n ",get_time_stamp(&philo->start),philo->philo_id);// dont print if someone died	// check here for death
	usleep(philo->philo_t_eat * 1000);
	philo->remaining_eats--;
	pthread_mutex_unlock(&philo->fork_mutex);
	pthread_mutex_unlock(&philo->next->fork_mutex);
}
void *routine(t_philo *philo)
{
	// check here for death

	while (1 || exiit (iddead))
	{
		printf("%li ms Philosopher %d is thinking\n",get_time_stamp(&philo->start),philo->philo_id);// dont print if someone died
		if (!dead)
			eating(philo);
		
		// check here for death
		usleep(1000);
		printf("%li ms Philosopher %d is sleeping\n",get_time_stamp(&philo->start),philo->philo_id);// dont print if someone died
		if (!dead )usleep(philo->philo_t_sleep * 1000);// maybe improve this fcking thing aka remove delay
		if (philo->remaining_eats < 0)
			return (NULL);
	}
	return (NULL);
}

/*
	sleep
	if (dead)
	{

	}
	if (time to sleep is up)
	{

	}
*/




/*


	while (1 && !dead)
	{
		if (state == fork)
			take forks
		if (state == eat)
			eat
		if (state == sleep)
			sleep
		if (state == think)
			think


		usleep(1000);
	}



*/


//how somethin died
// if doenst eat before

// when something dead 
// -> print RIP
// -> stop printing
// -> pthread join everyting


void	print(id, what to do)
{
	lock mutex;
	if (1)
		printf("iid is sleeping")
	if (2)
		printf("iid is eating")
	unlock mutex;

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

// void	wait_threads(t_philo *philos)
// {
// 	t_philo *tmp;

// 	tmp = philos;
// 	while (tmp)
// 	{
// 		int	k;
	
// 		k = pthread_join(philos->philo_thr, NULL);
// 		if(k!=0)
// 		{
// 			printf("\n Thread join failed \n");
// 			exit(1);
// 		}
// 		tmp = tmp->next;
// 		if (tmp == philos)
// 			break ;
// 	}
// }

// void	dstr_mutx(t_philo *philos)
// {
// 	t_philo *tmp;

// 	tmp = philos;
// 	while (tmp)
// 	{
// 		int	k;
	
// 		k = pthread_mutex_destroy(&philos->fork_mutex);
// 		if(k!=0)
// 		{
// 			printf("\n Thread join failed \n");
// 			exit(1);
// 		}
// 		tmp = tmp->next;
// 		if (tmp == philos)
// 			break ;
// 	}
// }

int main(int argc, char *argv[]) 
{
    int     n;
	t_philo *philos;
	t_philo *tmp;

 	gettimeofday(g_start,NULL);
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
		gettimeofday(&tmp->start, NULL);
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

	// system("leaks philosophers");
    return argc;
}