/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 08:44:41 by mjallada          #+#    #+#             */
/*   Updated: 2022/06/07 08:44:43 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/* Picks up forks in a certain order depending on whether the philo_id is even
 * or odd. Even philos will pick up their left fork first, odd philos will pick
 * up their right fork first. */

void	fork_pickup(t_philo *philo)
{
	int	philo_id;

	philo_id = philo->philo_id;
	if (philo_id % 2 == 0)
	{
		if (pthread_mutex_lock(philo->left_fork) != 0)
			exit(print_err("Failed to lock mutex", 1)
				&& freester(philo->rules_ptr, 1));
		printf("%ld %d has taken a fork (left)\n", get_timestamp(philo), philo_id);
//		printf("%ld %d has taken a fork\n", get_timestamp(philo), philo_id);
		if (philo->right_fork && pthread_mutex_lock(philo->right_fork) != 0)
			exit(print_err("Failed to lock mutex", 1)
				&& freester(philo->rules_ptr, 1));
		printf("%ld %d has taken a fork (right)\n", get_timestamp(philo), philo_id);
//		printf("%ld %d has taken a fork\n", get_timestamp(philo), philo_id);
	}
	else
	{
		if (philo->right_fork && pthread_mutex_lock(philo->right_fork) != 0)
			exit(print_err("Failed to lock mutex", 1)
				&& freester(philo->rules_ptr, 1));
		printf("%ld %d has taken a fork (right)\n", get_timestamp(philo), philo_id);
//		printf("%ld %d has taken a fork\n", get_timestamp(philo), philo_id);
		if (pthread_mutex_lock(philo->left_fork) != 0)
			exit(print_err("Failed to lock mutex", 1)
				&& freester(philo->rules_ptr, 1));
		printf("%ld %d has taken a fork (left)\n", get_timestamp(philo), philo_id);
//		printf("%ld %d has taken a fork\n", get_timestamp(philo), philo_id);
	}
}

/* Puts down forks in a certain order depending on whether the philo_id is even
 * or odd. Even philos will put down their left fork first, odd philos will
 * put down their right fork first. */
// TODO Fix exit strategy for when unlocking failed
void	fork_putdown(t_philo *philo)
{
	int	philo_id;

	philo_id = philo->philo_id;
	if (philo->philo_id % 2 == 0)
	{
		if (pthread_mutex_unlock(philo->left_fork) != 0)
			exit(print_err("Failed to unlock mutex", 1)
				&& freester(philo->rules_ptr, 1));
//		printf("%ld %d unlocked left fork\n", get_time() - philo->rules_ptr->start_time, philo->philo_id);
		if (pthread_mutex_unlock(philo->right_fork) != 0)
			exit(print_err("Failed to unlock mutex", 1)
				&& freester(philo->rules_ptr, 1));
//		printf("%ld %d unlocked right fork\n", get_time() - philo->rules_ptr->start_time, philo->philo_id);
	}
	else
	{
		if (pthread_mutex_unlock(philo->right_fork) != 0)
			exit(print_err("Failed to unlock mutex", 1)
				&& freester(philo->rules_ptr, 1));
//		printf("%ld %d unlocked right fork\n", get_time() - philo->rules_ptr->start_time, philo->philo_id);
		if (pthread_mutex_unlock(philo->left_fork) != 0)
			exit(print_err("Failed to unlock mutex", 1)
				&& freester(philo->rules_ptr, 1));
//		printf("%ld %d unlocked left fork\n", get_time() - philo->rules_ptr->start_time, philo->philo_id);
	}
}

/* Keeps if the first philo has already eaten, it waits for the last one
 * to eat the same amount of times before eating again */

void	save_last_philo(t_philo *philo)
{
	int		philo_count;
	t_philo	*last_philo;

	philo_count = (int)philo->rules_ptr->nb_of_philos;
	last_philo = philo->rules_ptr->philos[philo_count - 1];
	if (philo->philo_id == 0 && philo->nb_meals)
		while (last_philo->nb_meals != philo->nb_meals)
			usleep(1);
}

/* Returns current timestamp */

long int	get_timestamp(t_philo *philo)
{
	return (get_time() - philo->rules_ptr->start_time);
}
