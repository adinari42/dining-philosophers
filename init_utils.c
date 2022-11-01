/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:05:15 by adinari           #+#    #+#             */
/*   Updated: 2022/11/01 18:21:42 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*returns last node of the ll*/
t_philo	*ft_lstlast(t_philo *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

int	push(t_philo **thestack, int thevalue, char **argv, int argc)
{
	t_philo	*newnode;
	t_philo *temp;

	if (thestack == NULL)
		return (0);
	newnode = malloc(sizeof(t_philo));
	if (newnode == NULL)
	{
		write(2, "Error\n", 6);
		return (0);
	}
	newnode->philo_id = thevalue;
	newnode->philo_t_die = ft_atoi(argv[2]);
	newnode->philo_t_eat = ft_atoi(argv[3]);
	newnode->philo_t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		newnode->remaining_eats = ft_atoi(argv[5]);
	else
		newnode->remaining_eats = 0;
	pthread_mutex_init(&newnode->fork_mutex, NULL);
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
	return (1);
}

int	fill_ll(int philo_id, char **argv, t_philo **philos,int argc)
{
	t_philo	*tmp;

	push(philos, philo_id, argv, argc);
	tmp = ft_lstlast(*philos);
	free(tmp->next);
	tmp->next = NULL;
	// *philos->next = tmp;
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
	total_philo = atoi(argv[1]);
	while (++i < total_philo)
	{
		if (fill_ll(i + 1, argv, philos, argc))//check if cond is wron on nect -> we are last if not
			return (write(2, "Error\n", 6));
	}
	return (0);
}

t_philo    *init_philosophers(int argc, char **argv)
{
	t_philo *philos;

    if (init_junk(argv, argc, &philos) != 0)
	{
		perror("Error\n");
		exit(1);
	}
	t_philo	*tmp;
	tmp = ft_lstlast(philos);
	tmp->next = philos;
	return philos;

    //while loop to initialize every fork_mutex for each philo
}
