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

int	fork_pickup(t_philo *philo)
{
	int	philo_id;

	philo_id = philo->philo_id;
	if (philo_id % 2 == 0)
	{
		if (pthread_mutex_lock(philo->left_fork) != 0)
			return (print_err("Failed to lock mutex", 0));
//		print_status("has taken a fork (left)", philo);
		print_status("has taken a fork", philo);
		if (philo->rules_ptr->nb_of_philos == 1)
			return (1);
		if (philo->right_fork && pthread_mutex_lock(philo->right_fork) != 0)
			return (print_err("Failed to lock mutex", 0));
//		print_status("has taken a fork (right)", philo);
		print_status("has taken a fork", philo);
	}
	else
	{
		if (philo->right_fork && pthread_mutex_lock(philo->right_fork) != 0)
			return (print_err("Failed to lock mutex", 0));
//		print_status("has taken a fork (right)", philo);
		print_status("has taken a fork", philo);
		if (pthread_mutex_lock(philo->left_fork) != 0)
			return (print_err("Failed to lock mutex", 0));
//		print_status("has taken a fork (left)", philo);
		print_status("has taken a fork", philo);
	}
	return (1);
}

/* Puts down forks in a certain order depending on whether the philo_id is even
 * or odd. Even philos will put down their left fork first, odd philos will
 * put down their right fork first. */
// TODO Fix exit strategy for when unlocking failed
int	fork_putdown(t_philo *philo)
{
	int	philo_id;

	philo_id = philo->philo_id;
	if (philo->philo_id % 2 == 0)
	{
		if (pthread_mutex_unlock(philo->left_fork) != 0)
			return (print_err("Failed to unlock mutex", 0));
//		print_status("unlocked a fork (left)", philo);
		if (pthread_mutex_unlock(philo->right_fork) != 0)
			return (print_err("Failed to unlock mutex", 0));
//		print_status("unlocked a fork (right)", philo);
	}
	else
	{
		if (pthread_mutex_unlock(philo->right_fork) != 0)
			return (print_err("Failed to unlock mutex", 0));
//		print_status("unlocked a fork (right)", philo);
		if (pthread_mutex_unlock(philo->left_fork) != 0)
			return (print_err("Failed to unlock mutex", 0));
//		print_status("unlocked a fork (left)", philo);
	}
	return (1);
}

///* Keeps if the first philo has already eaten, it waits for the last one
// * to eat the same amount of times before eating again */
//
//void	save_last_philo(t_philo *philo)
//{
//	long int		philo_count;
//	t_philo			*last_philo;
//
//	philo_count = philo->rules_ptr->nb_of_philos;
//	last_philo = philo->rules_ptr->philos[philo_count - 1];
//	if (philo->philo_id == 0 && philo->nb_meals)
//		while (last_philo->nb_meals != philo->nb_meals)
//			usleep(1);
//}

/* Returns current timestamp */

long int	get_timestamp(t_philo *philo)
{
	return (get_time() - philo->rules_ptr->start_time);
}
