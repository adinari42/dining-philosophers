/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:05:15 by adinari           #+#    #+#             */
/*   Updated: 2022/11/08 21:25:14 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	push(t_philo **thestack, int thevalue, char **argv, int argc)
{
	t_philo	*newnode;
	t_philo	*temp;

	if (thestack == NULL)
		return (0);
	newnode = malloc(sizeof(t_philo));
	if (newnode == NULL)
	{
		write(2, "Error\n", 6);
		return (1);
	}
	set_values(newnode, thevalue, argv);
	reduce_long_time(newnode);
	set_remaining_eats(newnode, argc, argv);
	newnode->next = NULL;
	if (*thestack == NULL)
		*thestack = newnode;
	else
	{
		temp = *thestack;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = newnode;
	}
	return (0);
}

int	fill_ll(int philo_id, char **argv, t_philo **philos, int argc)
{
	t_philo	*tmp;

	if (push(philos, philo_id, argv, argc))
		return (1);
	tmp = ft_lstlast(*philos);
	free(tmp->next);
	tmp->next = NULL;
	return (0);
}

//initializes linked list a with parameters argv,
//prints error in case of failure
//returns 1 in case of duplicates
int	init_junk(char **argv, int argc, t_philo **philos)
{
	int	i;
	int	total_philo;

	i = -1;
	total_philo = ft_atoi(argv[1]);
	while (++i < total_philo)
	{
		if (fill_ll(i + 1, argv, philos, argc))
			return (write(2, "Error\n", 6));
	}
	return (0);
}

int	init_monitor(t_philo *philo, char **argv)
{
	t_data	*monitor;
	t_philo	*tmp;

	monitor = malloc(sizeof(t_data));
	if (monitor == NULL)
	{
		write(2, "Error\n", 6);
		return (1);
	}
	pthread_mutex_init(&monitor->print_mutex, NULL);
	monitor->isdead = 0;
	monitor->iscreated = 0;
	monitor->total_p = ft_atoi(argv[1]);
	tmp = philo;
	while (tmp)
	{
		tmp->monitor = monitor;
		tmp = tmp->next;
		if (tmp == philo)
			break ;
	}
	return (0);
}

t_philo	*init_philosophers(int argc, char **argv)
{
	t_philo	*philos;
	t_philo	*tmp;

	if (init_junk(argv, argc, &philos) != 0)
	{
		write(2, "Error\n", 6);
		return (NULL);
	}
	tmp = ft_lstlast(philos);
	tmp->next = philos;
	if (init_monitor(philos, argv))
		return (NULL);
	return (philos);
}
