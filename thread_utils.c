/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 22:31:40 by adinari           #+#    #+#             */
/*   Updated: 2022/11/08 20:33:49 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	long	val;
	int		sign;
	int		i;

	val = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		val = (val * 10) + (str[i++] - '0');
		if (val > 2147483647 && sign == 1)
			return (-1);
		if (val > 2147483648 && sign == -1)
			return (0);
	}
	return (val * sign);
}

int	init_forks(t_philo *philos)
{
	t_philo	*tmp;

	tmp = philos;
	while (tmp)
	{
		if(pthread_mutex_init(&tmp->fork_mutex,NULL) == -1)
		{
			printf("\nmutex initialized");
			return (1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	return (0);
}

int	start_dining(t_philo *philos)
{
	pthread_mutex_init(&philos->monitor->creation_mutex, NULL);
	pthread_mutex_lock(&philos->monitor->creation_mutex);
	philos->monitor->iscreated = 1;
	if(death_monitor_thread(philos))
		return(1);
	pthread_mutex_unlock(&philos->monitor->creation_mutex);
	return (0);
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

int	death_monitor_thread(t_philo *philo)
{
	if(pthread_create(&philo->monitor->death_monit, NULL, (void *)&check_death, philo))
	{
		printf("\n Thread creation error \n");
		return (1);
	}
	return (0);
}

int	init_philo_threads(t_philo *philos)
{
	t_philo	*tmp;

	tmp = philos;
	while (tmp)
	{
		gettimeofday(&tmp->start, NULL);
		philos->last_eat = get_time_stamp(&tmp->start);
		if(pthread_create(&tmp->philo_thr, NULL, (void *)&routine, tmp)!=0)
		{
			printf("\n Thread creation error \n");
			return (1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	return (0);
}
