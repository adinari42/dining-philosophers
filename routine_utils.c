/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 20:48:46 by adinari           #+#    #+#             */
/*   Updated: 2022/11/07 21:10:56 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// from sec to ms, from microsec to ms
long long	ft_gettime(void)
{
	struct timeval	time_value;
	long long		time;

	gettimeofday(&time_value, NULL);
	time = time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
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
			printf("%li %d has taken a fork\n", philo->last_eat,
				philo->philo_id);
			printf("%li %d is eating\n", philo->last_eat,
				philo->philo_id);
		}
		if (philo->state == 2)
			printf("%li %d is sleeping\n", get_time_stamp(&philo->start),
				philo->philo_id);
		if (philo->state == 3 && philo->begin == 0)
			printf("%li %d is thinking\n", get_time_stamp(&philo->start),
				philo->philo_id);
		else if (philo->begin == 1)
			philo->begin = 0;
		if (philo->state == 5)
			printf("%li %d has taken a fork\n", get_time_stamp(&philo->start),
				philo->philo_id);
	}
	pthread_mutex_unlock(&philo->monitor->print_mutex);
}

void	eating(t_philo *philo)
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
	while (philo->monitor->iscreated == 0)
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
