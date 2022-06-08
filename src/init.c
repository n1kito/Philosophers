/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 09:08:51 by mjallada          #+#    #+#             */
/*   Updated: 2022/06/07 09:08:53 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/* Initializes the basic values in each philosopher struct */
// TODO Replace with the following : rules->philos[i]->philo_id = i + 1;

void	init_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_of_philos)
	{
		rules->philos[i]->rules_ptr = rules;
		rules->philos[i]->nb_meals = 0;
		rules->philos[i]->last_meal = 0;
		rules->philos[i]->philo_id = i;
		rules->philos[i]->is_done_eating = 0;
		i++;
	}
}

/* Sets up all the forks in the forks array then goes through each philo
 * structure and assigns their pointers to their left/right forks. */

int	init_and_assign_forks(t_rules *rules)
{
	int	i;
	int	ph_nb;

	ph_nb = (int)rules->nb_of_philos;
	i = 0;
	while (i < rules->nb_of_philos)
	{
		if (pthread_mutex_init(rules->forks[i], NULL) != 0)
			return (print_err("Failed to initiate mutex", 0));
		i++;
	}
	i = 0;
	while (i < rules->nb_of_philos)
	{
		rules->philos[i]->left_fork = rules->forks[i];
		if (rules->nb_of_philos == 1)
			break ;
		rules->philos[i]->right_fork = rules->forks[(i + 1) % ph_nb];
		i++;
	}
	return (1);
}
