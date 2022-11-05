/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:53:15 by adinari           #+#    #+#             */
/*   Updated: 2022/11/05 22:41:12 by adinari          ###   ########.fr       */
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
long long	ft_gettime(void)
{
	struct timeval	time_value;
	long long		time;

	gettimeofday(&time_value, NULL);
	time = time_value.tv_sec * 1000 + time_value.tv_usec / 1000;// from sec to ms, from microsec to ms
	return (time);
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = ft_gettime();
	while ((ft_gettime() - start_time) < time_in_ms)
		usleep(200);
}

void	print(t_philo *philo)
{
	pthread_mutex_lock(&philo->monitor->print_mutex);
	if (philo->monitor->isdead)
		pthread_mutex_lock(&philo->monitor->death_mutex);
	if (philo->state == 1)
	{
		printf("%li ms Philosopher %d took a fork\n",philo->last_eat,philo->philo_id);
		printf("%li ms Philosopher %d is eating\n",philo->last_eat ,philo->philo_id);
	}
	if (philo->state == 2)
		printf("%li ms Philosopher %d is sleeping\n",get_time_stamp(&philo->start),philo->philo_id);
	if (philo->state == 3)
		printf("%li ms Philosopher %d is thinking\n",get_time_stamp(&philo->start),philo->philo_id);
	if (philo->monitor->isdead)
		pthread_mutex_unlock(&philo->monitor->death_mutex);
	pthread_mutex_unlock(&philo->monitor->print_mutex);
}

void join_threads(t_philo *philo)
{
	t_philo *tmp;
	int		k;

	tmp = philo;
	printf("joining threads, death = %d\n", philo->monitor->isdead);
	while (tmp)
	{
		k = pthread_join(tmp->philo_thr,NULL);
		if (k != 0)
		{
			printf("\n Thread join failed \n");
			exit(1);
		}
		tmp = tmp->next;
		if (tmp == philo)
			break ;
	}
}

void	destroy_mutexes(t_philo *philos)
{
	t_philo	*tmp;
	int		k;

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
	k=pthread_mutex_destroy(&tmp->monitor->death_mutex);
	if(k!=0)
	{
		printf("\n Mutex Destroyed \n");
		exit(1);
	}
	k=pthread_mutex_destroy(&tmp->monitor->print_mutex);
	if(k!=0)
	{
		printf("\n Mutex Destroyed \n");
		exit(1);
	}
}

void	check_death(t_philo *philo)
{
	t_philo *tmp;

	tmp = philo;
	while (tmp && tmp->monitor->isdead != 1)
	{
		if (get_time_stamp(&philo->start) - philo->last_eat > philo->philo_t_die)
		{
			philo->monitor->isdead = 1;
			printf("Philosopher %d has died\n", philo->philo_id);
			join_threads(philo);
			break;
		}
		tmp = tmp->next;
		if (tmp == philo)
			break ;
	}
	
}

int g_int = 0;
struct timeval *g_start = NULL;
void eating(t_philo *philo)
{
// printf  to  get forks and 	// check here for death
	check_death(philo);
	pthread_mutex_lock(&philo->fork_mutex);// prevent deadlock
	printf("%li ms Philosopher %d took a fork\n",get_time_stamp(&philo->start),philo->philo_id);
// printf  to  get forks and 	// check here for death
	check_death(philo);
	pthread_mutex_lock(&philo->next->fork_mutex);// prevent deadlock
	philo->state = 1;
	philo->last_eat = get_time_stamp(&philo->start);
	print(philo);// dont print if someone died	// check here for death
	ft_usleep(philo->philo_t_eat);
	// if (philo->monitor->infinite == 0)
	philo->remaining_eats--;
	pthread_mutex_unlock(&philo->fork_mutex);
	pthread_mutex_unlock(&philo->next->fork_mutex);
}
void *routine(t_philo *philo)
{
	while(philo->monitor->iscreated == 0)
		ft_usleep(1);
	// check here for death
	check_death(philo);
	// if (philo->philo_id % 2 == 0)
	// 	ft_usleep(philo->philo_t_eat / 2);
	while (!philo->monitor->isdead)
	{
		philo->state = 3;//thinking
		print(philo);
		if (!philo->monitor->isdead)//eating
			eating(philo);
		check_death(philo);
		if (!philo->monitor->isdead)
		{	philo->state = 2;//sleeping
			print(philo);
			ft_usleep(philo->philo_t_sleep);}// maybe improve this fcking thing aka remove delay
		if (philo->remaining_eats == 0 || philo->monitor->isdead)
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
		printf("philo id = %d , die time = %d, eat time = %d, sleep time = %d , remain eats = %d, monitor = %p\n",tmp->philo_id, tmp->philo_t_die, tmp->philo_t_eat, tmp->philo_t_sleep, tmp->remaining_eats, tmp->monitor);
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	printf("is dead = %d\n", philos->monitor->isdead);
}

void check(void)
{
	system("leaks philosophers");
}

int main(int argc, char *argv[]) 
{
    int     n;
	t_philo *philos;
	t_philo *tmp;
	int k;

	// atexit(check);
 	
	gettimeofday(g_start,NULL);
	if (argc < 5 || argc > 6)//use is_digit on args
	{
		perror("Error : wrong number of arguments\n");
		exit(1);
	}
    n = atoi(argv[1]);
    philos = init_philosophers(argc, argv);
	display_list(philos);
	tmp = philos;
	while (tmp)
	{
		k=pthread_mutex_init(&tmp->fork_mutex,NULL);
		if(k == -1)
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
		philos->last_eat = get_time_stamp(&tmp->start);
		k=pthread_create(&tmp->philo_thr, NULL, (void *)&routine, tmp);
		if(k!=0)
		{
			printf("\n Thread creation error \n");
			exit(1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	pthread_mutex_lock(&philos->monitor->creation_mutex);
	philos->monitor->iscreated = 1;
	pthread_mutex_unlock(&philos->monitor->creation_mutex);
	if (!philos->monitor->isdead)
		join_threads(philos);
	destroy_mutexes(philos);
	// system("leaks philosophers");
    return (0);
}