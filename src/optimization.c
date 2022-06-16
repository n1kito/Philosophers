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
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(FORK, philo);
		if (philo->rules->nb_of_philos == 1)
		{
			pthread_mutex_unlock(philo->left_fork);
			return ;
		}
//		if (philo->right_fork) //TODO PK G MIS CA ?
			pthread_mutex_lock(philo->right_fork);
		print_status(FORK, philo);
	}
	else
	{
//		if (philo->right_fork) //TODO PK G MIS CA ?
			pthread_mutex_lock(philo->right_fork);
		print_status(FORK, philo);
		pthread_mutex_lock(philo->left_fork);
		print_status(FORK, philo);
	}
}

/* Puts down forks in a certain order depending on whether the philo_id is even
 * or odd. Even philos will put down their left fork first, odd philos will
 * put down their right fork first. */
// TODO Fix exit strategy for when unlocking failed
void	fork_putdown(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
//		print_status("unlocked a fork (left)", philo);
//		print_status("unlocked a fork (right)", philo);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
//		print_status("unlocked a fork (right)", philo);
//		print_status("unlocked a fork (left)", philo);
	}
}
