/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 20:51:39 by adinari           #+#    #+#             */
/*   Updated: 2022/11/08 20:57:59 by adinari          ###   ########.fr       */
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

/*returns last node of the ll*/
t_philo	*ft_lstlast(t_philo *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

int	join_threads(t_philo *philo)
{
	t_philo	*tmp;

	tmp = philo;
	while (tmp)
	{
		if (tmp->state != 4)
		{
			if (pthread_join(tmp->philo_thr, NULL))
			{
				printf("\n Thread join failed \n");
				return (1);
			}
			tmp->state = 4;
		}	
		tmp = tmp->next;
		if (tmp == philo)
			break ;
	}
	if (pthread_join(tmp->monitor->death_monit, NULL))
	{
		printf("\ndeath monit Thread join failed \n");
		return (1);
	}
	return (0);
}

int	destroy_mutexes(t_philo *philos)
{
	t_philo	*tmp;

	tmp = philos;
	while (tmp)
	{
		if (pthread_mutex_destroy(&tmp->fork_mutex))
		{
			printf("\n Error : mutex destroy fail \n");
			return (1);
		}
		tmp = tmp->next;
		if (tmp == philos)
			break ;
	}
	if (pthread_mutex_destroy(&tmp->monitor->print_mutex) != 0)
	{
		printf("\n Error : mutex destroy fail \n");
		return (1);
	}
	if (pthread_mutex_destroy(&tmp->monitor->creation_mutex))
	{
		printf("\n Error : mutex destroy fail \n");
		return (1);
	}
	return (0);
}
