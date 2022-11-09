/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 20:57:46 by adinari           #+#    #+#             */
/*   Updated: 2022/11/08 21:27:55 by adinari          ###   ########.fr       */
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

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9' )
	{
		return (c);
	}
	else
		return (0);
}

void	set_values(t_philo *newnode, int thevalue, char **argv)
{
	newnode->philo_id = thevalue;
	newnode->philo_t_die = ft_atoi(argv[2]);
	newnode->philo_t_eat = ft_atoi(argv[3]);
	newnode->philo_t_sleep = ft_atoi(argv[4]);
}

void	reduce_long_time(t_philo *newnode)
{
	if (newnode->philo_t_sleep > newnode->philo_t_die)
		newnode->philo_t_sleep = newnode->philo_t_die + 50;
	if (newnode->philo_t_eat > newnode->philo_t_die)
		newnode->philo_t_eat = newnode->philo_t_die + 50;
}

void	set_remaining_eats(t_philo	*newnode, int argc, char **argv)
{
	if (argc == 6)
		newnode->remaining_eats = ft_atoi(argv[5]);
	else
		newnode->remaining_eats = -1;
}
