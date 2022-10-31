/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pseudocode1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:50:28 by adinari           #+#    #+#             */
/*   Updated: 2022/10/31 14:44:08 by adinari          ###   ########.fr       */
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

void    init_philosophers(int total_ph)
{
    pthread_t philo[total_ph];
    pthread_mutex_t mutex;
    int k;
    
    pthread_mutex_init(&mutex, NULL);
    for (k = 1; k <= 5; k++)
    {   
        if (pthread_create(&philo[k], NULL, (void *)&routine, &mutex) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
        printf("Thread %d has started\n", k);
    }
    k = 1;
    for (k = 1; k <= 5; k++) {
        if (pthread_join(philo[k], NULL) != 0) {
            exit(2);
        }
        printf("Thread %d has finished execution   \n", k); 
    }
    pthread_mutex_destroy(&mutex);
    printf("end : i = %d\n", i);
}

int main(int argc, char *argv[]) 
{
    int     n;

    n = atoi(argv[1]);
    
    init_philosophers(n);
    return argc;
}