/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:53:15 by adinari           #+#    #+#             */
/*   Updated: 2022/11/08 20:33:06 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9' )
	{
		return (c);
	}
	else
		return (0);
}

int	parse_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		write(2, "Error : wrong number of arguments\n", 34);
		return (1);
	}
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_isdigit(argv[i][j]) == 0)
			{
				write(2, "Error : argument incorrect format/type\n", 39);
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	join_and_destroy(t_philo *philos)
{
	if (!philos->monitor->isdead)
	{
		if (join_threads(philos))
			return (1);
	}
	if (destroy_mutexes(philos))
		return (1);
	return (0);
}

int	single_ph_check(t_philo *philos)
{
	if (philos->monitor->total_p == 1)
	{
		printf("0 %d took a fork\n", philos->philo_id);
		ft_usleep(philos->philo_t_die);
		printf("%d %d has died\n", philos->philo_t_die, philos->philo_id);
		return (1);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_philo	*philos;

	if (parse_args(argc, argv))
		return (1);
	philos = init_philosophers(argc, argv);
	if (single_ph_check(philos))
		return (1);
	if (init_forks(philos))
		return (1);
	if (init_philo_threads(philos))
		return (1);
	pthread_mutex_init(&philos->monitor->creation_mutex, NULL);
	pthread_mutex_lock(&philos->monitor->creation_mutex);
	philos->monitor->iscreated = 1;
	if (death_monitor_thread(philos))
		return (1);
	pthread_mutex_unlock(&philos->monitor->creation_mutex);
	if (join_and_destroy(philos))
		return (1);
	return (0);
}
