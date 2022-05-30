/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 09:14:57 by mjallada          #+#    #+#             */
/*   Updated: 2022/05/30 09:15:11 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/* Routine that each philo has to follow */

void	*routine(void *rules_tmp)
{
	t_rules	*rules;

	rules = (t_rules *)rules_tmp;

}

/* Starts philos/threads one at a time */

int	launch_philos(t_rules *rules)
{
	int	i;
	int	philo_count;

	philo_count = rules->nb_of_philos;
	i = 0;
	while (i < philo_count)
	{
		if (pthread_create(rules->philos[i]->philo, NULL, &routine, rules) != 0)
			return (print_err("Failed to create philo"), 0);
		i++;
	}
}

/* Initializes the basic values in each philosopher struct */

void	init_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_of_philos)
	{
		rules->philos[i]->rules_ptr = rules;
		rules->philos[i]->nb_meals = 0;
		rules->philos[i]->last_meal = 0;
		rules->philos[i]->somebody_died_ptr = &(rules->somebody_died);
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
		if (pthread_mutex_init(rules->forks[i++], NULL) != 0)
			return (print_err("Failed to initiate mutex"), 0);
	i = 0;
	while (i < rules->nb_of_philos)
	{
		rules->philos[i]->left_fork = rules->forks[i];
		if (rules->nb_of_philos == 1)
			break ;
		rules->philos[i]->right_fork = rules->forks[(i + 1) % ph_nb]; // check that this is correct calculation
		i++;
	}
	return (1);
}
