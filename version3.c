/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:53:15 by adinari           #+#    #+#             */
/*   Updated: 2022/11/07 04:58:48 by adinari          ###   ########.fr       */
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


//1 eating //2 sleeping //3 thinking //4 dead //5 took fork
void	print(t_philo *philo)
{
	pthread_mutex_lock(&philo->monitor->print_mutex);
	if (philo->monitor->isdead != 1)
	{
		if (philo->state == 1)
		{
			printf("%li %d has taken a fork\n",philo->last_eat,philo->philo_id);
			printf("%li %d is eating\n",philo->last_eat ,philo->philo_id);
		}
		if (philo->state == 2)
			printf("%li %d is sleeping\n",get_time_stamp(&philo->start),philo->philo_id);
		if (philo->state == 3 && philo->begin == 0)
			printf("%li %d is thinking\n",get_time_stamp(&philo->start),philo->philo_id);
		else if (philo->begin == 1)
			philo->begin = 0;
		if (philo->state == 5)
			printf("%li %d has taken a fork\n",get_time_stamp(&philo->start),philo->philo_id);
	}
	pthread_mutex_unlock(&philo->monitor->print_mutex);
}

void join_threads(t_philo *philo)
{
	t_philo	*tmp;
	int		k;

	tmp = philo;
	printf("joining threads, death = %d\n", philo->monitor->isdead);
	while (tmp)
	{
		if (tmp->state != 4)
		{
			printf("inside join\n");
			k = pthread_join(tmp->philo_thr, NULL);
			if (k != 0)
			{
				printf("\n Thread join failed \n");
				return ;
			}
			tmp->state = 4;
		}	
		tmp = tmp->next;
		if (tmp == philo)
			break ;
	}
	k = pthread_join(tmp->monitor->death_monit, NULL);
	if (k != 0)
	{
		printf("\ndeath monit Thread join failed \n");
		return ;
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

void	*check_death(t_philo *philo)
{
	t_philo *tmp;

	tmp = philo;
	if (tmp->monitor->isdead != 1)
	{
		while (tmp)
		{
			if (get_time_stamp(&philo->start) - philo->last_eat > philo->philo_t_die)
			{
				philo->monitor->isdead = 1;
				if (philo->remaining_eats != 0)
					printf("%ld %d has died\n", philo->last_eat + philo->philo_t_die ,philo->philo_id);
				break;
			}
			tmp = tmp->next;
		}
	}
	return (NULL);
}

void	death_monitor_thread(t_philo *philo)
{
	int	k;

	k = pthread_create(&philo->monitor->death_monit, NULL, (void *)&check_death, philo);
	if(k != 0)
	{
		printf("\n Thread creation error \n");
		exit(1);
	}
}

int g_int = 0;
struct timeval *g_start = NULL;
void eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_mutex);
	philo->state = 5;
	print(philo);
	if (philo->philo_id != philo->next->philo_id)
	{
		pthread_mutex_lock(&philo->next->fork_mutex);
		philo->state = 1;
		philo->last_eat = get_time_stamp(&philo->start);
		print(philo);
		ft_usleep(philo->philo_t_eat);
		philo->remaining_eats--;
		pthread_mutex_unlock(&philo->next->fork_mutex);
	}
	pthread_mutex_unlock(&philo->fork_mutex);
}

void	*routine(t_philo *philo)
{
	philo->begin = 1;
	while(philo->monitor->iscreated == 0)
		ft_usleep(1);
	if (philo->philo_id % 2 == 0)
		ft_usleep(philo->philo_t_eat / 2);
	while (!philo->monitor->isdead)
	 {	
		philo->state = 3;
		print(philo);
		if (!philo->monitor->isdead)
			eating(philo);
		if (!philo->monitor->isdead)
		{
			philo->state = 2;
			print(philo);
			ft_usleep(philo->philo_t_sleep);
		}
		if (philo->remaining_eats == 0 || philo->monitor->isdead)
			return (NULL);
	}
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
	t_philo	*tmp;

	tmp = philos;
	while (tmp)
	{
		printf("philo id = %d, next id = %d , die time = %d, eat time = %d, sleep time = %d , remain eats = %d, monitor = %p\n",tmp->philo_id, tmp->next->philo_id, tmp->philo_t_die, tmp->philo_t_eat, tmp->philo_t_sleep, tmp->remaining_eats, tmp->monitor);
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	printf("is dead = %d\n", philos->monitor->isdead);
}

int	parse_args(int argc, char **argv)
{
	int	i;
	int j;

	i = 1;
	if (argc < 5 || argc > 6)//use is_digit on args
	{
		write(2, "Error : wrong number of arguments\n", 34);
		return (1);
	}
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			// printf("%c", argv[i][j]);
			if (ft_isdigit(argv[i][j]) == 0)
			{
				write(2, "Error : incorrect format/type\n", 30);
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int main(int argc, char *argv[]) 
{
    int		n;
	t_philo	*philos;
	t_philo	*tmp;
	int		k;

	if(parse_args(argc, argv))
		return (1);
	gettimeofday(g_start,NULL);
    n = atoi(argv[1]);//fix to ft_atoi
    philos = init_philosophers(argc, argv);
	if (philos->monitor->total_p == 1)
	{
		printf("0 %d took a fork\n",philos->philo_id);
		if (philos->philo_t_die < 0)
			philos->philo_t_die *= -1;
		ft_usleep(philos->philo_t_die);
		printf("%d %d has died\n", philos->philo_t_die ,philos->philo_id);
		return (1);
	}
	//display_list(philos);
	tmp = philos;
	while (tmp)
	{
		k=pthread_mutex_init(&tmp->fork_mutex,NULL);
		if(k == -1)
		{
			printf("\nmutex initialized");
			return (1);
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
			return (1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	pthread_mutex_lock(&philos->monitor->creation_mutex);
	philos->monitor->iscreated = 1;
	death_monitor_thread(philos);
	pthread_mutex_unlock(&philos->monitor->creation_mutex);
	if (!philos->monitor->isdead)
		join_threads(philos);
	destroy_mutexes(philos);
	// system("leaks philosophers");
    return (0);
}