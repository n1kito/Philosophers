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

/* Goes through all philos and checks if they've eaten recently enough. */

void	*check_dead_philo(void *rules_tmp)
{
	t_rules	*rules;
	int	i;

	rules = (t_rules *)rules_tmp;
	while (1)
	{
		i = 0;
		while (i < rules->nb_of_philos)
		{
//			printf("\033[0;33m**checking for a dead cunt\n\033[0m");
			if ((get_time() - rules->start_time) - rules->philos[i]->last_meal > rules->t_to_die)
			{
//				printf("\033[0;31m** %ld philo %d DIED after waiting for %ld **\033[0m\n", get_time() - rules->start_time, rules->philos[i]->philo_id, (get_time() - rules->start_time) - rules->philos[i]->last_meal);
				printf("%ld %d died\n", get_timestamp(rules->philos[i]),
					rules->philos[i]->philo_id);
				exit (freester(rules, 1));
			}
			i++;
//			else
//				printf("\033[0;32m**No philo has died\n\033[0m");
		}
	}
}

/* Checks if each philo has eaten the max number of times they should have */

void	check_number_of_meals(t_rules *rules)
{
	int	has_eaten_enough;
	int	i;

//	printf("\n********* checking MEALS BITCH *********\n");
	has_eaten_enough = 0;
	i = 0;
	while (i < rules->nb_of_philos)
	{
//		printf("* philo %d ate %d times\n", i, rules->philos[i]->nb_meals);
		if (rules->philos[i]->nb_meals == rules->min_meals)
			has_eaten_enough++;
		i++;
	}
//	printf("***************************************\n\n");
	if (has_eaten_enough == rules->nb_of_philos)
	{
//		printf("\033[0;32mDINNER'S OVER FUCKERS\033[0m ");
		printf("(%d philos ate %d servings each)\n", has_eaten_enough, (int)rules->min_meals);
		exit (freester(rules, 0)); // I need to free shit here.
	}
}

/* Routine that each philo has to follow */

void	*routine(void *philo_tmp)
{
	t_philo	*philo;

	philo = (t_philo *)philo_tmp;
	save_last_philo(philo);
	while (1)
	{
		if (philo->rules_ptr->min_meals == -1 || (philo->rules_ptr->min_meals
				&& philo->nb_meals < philo->rules_ptr->min_meals))
		{
			fork_pickup(philo);
			printf("%ld %d is eating\n", get_timestamp(philo), philo->philo_id);
			philo->last_meal = get_time();
			usleep(philo->rules_ptr->t_to_eat * 1000);
			philo->nb_meals++;
//			printf("philo %d has eaten\n", philo->philo_id);
			if (philo->rules_ptr->min_meals)
				check_number_of_meals(philo->rules_ptr);
			fork_putdown(philo);
			printf("%ld %d is thinking\n", get_timestamp(philo), philo->philo_id);
			usleep(philo->rules_ptr->t_to_eat * 1000 * 0.9);
			printf("%ld %d is sleeping\n", get_timestamp(philo), philo->philo_id);
			usleep(philo->rules_ptr->t_to_sleep * 1000);
		}
	}
}

/* Starts philos/threads one at a time */

int	launch_philos(t_rules *rules)
{
	int	i;
	int	philo_count;

	philo_count = (int)rules->nb_of_philos;
	i = 0;
	while (i < philo_count)
	{
		if (i % 2 == 0)
		{
//			printf("%ld Initiate even philo %d\n", get_time() - rules->start_time, i);
			if (pthread_create(&rules->philos[i]->philo, NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
		}
		i++;
	}
	usleep(rules->t_to_eat * 1000 * 0.9);
	i = 0;
	while (i < philo_count)
	{
		if (i % 2 != 0)
		{
//			printf("%ld Initiate odd philo %d\n", get_time() - rules->start_time, i);
			if (pthread_create(&rules->philos[i]->philo, NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
		}
		i++;
	}
	i = 0;
	while (i < rules->nb_of_philos)
		pthread_join(rules->philos[i++]->philo, NULL);
	return (1);
}
