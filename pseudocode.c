/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pseudocode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:50:28 by adinari           #+#    #+#             */
/*   Updated: 2022/10/31 17:45:47 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int i = 0;

void *routine(pthread_mutex_t *mutex)
{
    static int  count = 1;

    pthread_mutex_lock(mutex);
    printf("routine exec, count = %d\n", count);
    count++;
    pthread_mutex_unlock(mutex);
    return (NULL);
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

int	push(t_philo **thestack, int thevalue)
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

int	fill_ll(int philo_id, t_philo **philos)
{
	t_philo	*tmp;

	push(philos, philo_id);
	tmp = ft_lstlast(*philos);
	free(tmp->next);
	tmp->next = NULL;
	return (0);
}
//initializes linked list a with parameters argv,
//prints error in case of failure
//returns 1 in case of duplicates
int	init_junk(int total_ph, t_philo **philos)
{
	int	i;

	i = 0;
	
	while (++i < total_ph)
		if (fill_ll(i + 1, philos))
			return (write(2, "Error\n", 6));

	return (0);
}

void    init_philosophers(int total_ph)
{
    t_philo *philos;

    if (init_junk(total_ph, &philos) != 0)
	{
		perror("Error\n");
		exit(1);
	}
	while (philos)
	{
		printf("philo id = %d\n", philos->philo_id);
		philos = philos->next;
	}
    //while loop to initialize every fork_mutex for each philo
}

int main(int argc, char *argv[]) 
{
    int     n;

    n = atoi(argv[1]);
    init_philosophers(n);
	
    return argc;
}