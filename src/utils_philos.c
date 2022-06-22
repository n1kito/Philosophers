/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 12:45:58 by mjallada          #+#    #+#             */
/*   Updated: 2022/06/22 12:46:14 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Frees memory and destroys mutexes/forks if they've been initialized.

int	freester(t_rules *rules, int return_value)
{
	int	i;

	i = -1;
	if (rules->philos)
	{
		while (++i < rules->nb_of_philos)
			if (rules->philos[i])
				free(rules->philos[i]);
		free(rules->philos);
	}
	i = -1;
	if (rules->forks)
	{
		while (++i < rules->nb_of_philos)
		{
			if (rules->forks[i])
			{
				if (i < rules->initialized_mutexes)
					pthread_mutex_destroy(rules->forks[i]);
				free(rules->forks[i]);
			}
		}
		free(rules->forks);
	}
	return (return_value);
}

/* Protects my messages being printed and only prints if nobody died (except for
 * the message that announced the death of a philo) and if all philos are not
 * done eating. */

int	print_status(char *status, t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->printer_m);
	pthread_mutex_lock(&philo->rules->full_dinners_m);
	pthread_mutex_lock(&philo->rules->someone_died_m);
	if ((philo->rules->someone_died == 0
			|| (philo->rules->someone_died == 1
				&& ft_strstr(status, "died"))))
		printf(STATUS, get_timestamp(philo),
			philo->philo_id, status);
	pthread_mutex_unlock(&philo->rules->full_dinners_m);
	pthread_mutex_unlock(&philo->rules->someone_died_m);
	pthread_mutex_unlock(&philo->rules->printer_m);
	return (1);
}

/* uleep() the requested timme in increments of 100 microseconds */

void	opti_sleep(long int time)
{
	long int	start_time;

	start_time = get_time();
	while (get_time() - start_time < time)
		usleep(100);
}

/* Returns current timestamp in ms */

long int	get_timestamp(t_philo *philo)
{
	return (get_time() - philo->rules->start_time);
}
