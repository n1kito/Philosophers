/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 11:44:45 by mjallada          #+#    #+#             */
/*   Updated: 2022/05/25 11:44:52 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void	*routine(void *arg)
{
	sleep(1);
	int index = *(int *)arg;
	printf("%d ", primes[index]);
	free(arg);
}

int main(void)
{
	pthread_t threads[10];

	for (int i = 0; i < 10; i++)
	{
		int *a = malloc(sizeof(int));
		*a = 1;
		if (pthread_create(&threads[i], NULL, a, &i))
			perror("Failed to create thread");
	};
	for (int i = 0; i < 10; i++)
	{
		if(pthread_join(threads[i], NULL))
			perror("Failed to join thread");
	}

	return (0);
}
