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
	int		i;

	rules = (t_rules *)rules_tmp;
	while (!rules->someone_died
		&& rules->full_dinners != rules->nb_of_philos)
	{
		i = 0;
		while (i < rules->nb_of_philos && !rules->someone_died)
		{
//			printf("\033[0;33m**checking for a dead cunt\n\033[0m");
			if (get_timestamp(rules->philos[i]) - rules->philos[i]->last_meal > rules->t_to_die)
			{
//				printf("\033[0;31m** %ld philo %d DIED after waiting for %ld **\033[0m\n", get_time() - rules->start_time, rules->philos[i]->philo_id, (get_time() - rules->start_time) - rules->philos[i]->last_meal);
//				printf("%ld %d died\n", get_timestamp(rules->philos[i]),
//					rules->philos[i]->philo_id);
				print_status("died", rules->philos[i]);
				rules->someone_died = 1;
			}
			i++;
//			else
//				printf("\033[0;32m**No philo has died\n\033[0m");
		}
	}
	return (NULL);
}

///* Checks if each philo has eaten the max number of times they should have */
//
//int	has_eaten_enough(t_rules *rules)
//{
//	int	has_eaten_enough;
//	int	i;
//
////	printf("\n********* checking MEALS BITCH *********\n");
//	has_eaten_enough = 0;
//	i = 0;
//	while (i < rules->nb_of_philos)
//	{
////		printf("* philo %d ate %d times\n", i, rules->philos[i]->nb_meals);
//		if (rules->philos[i]->nb_meals == rules->min_meals)
//			has_eaten_enough++;
//		i++;
//	}
////	printf("***************************************\n\n");
//	if (has_eaten_enough == rules->nb_of_philos)
//	{
////		printf("\033[0;32mDINNER'S OVER FUCKERS\033[0m ");
//		printf("(%d philos ate %d servings each)\n", has_eaten_enough, (int)rules->min_meals);
//		return (1);
//	}
//	return (0);
//}

/* Checks if each philo has eaten the max number of times they should have */

void	has_eaten_enough(t_philo *philo)
{
	if (philo->nb_meals == philo->rules_ptr->min_meals)
	{
		philo->is_done_eating = 1;
		philo->rules_ptr->full_dinners++;
	}
}

/* Routine that each philo has to follow */

void	*routine(void *philo_tmp)
{
	t_philo	*philo;

	philo = (t_philo *)philo_tmp;
//	save_last_philo(philo); // j'ai pas le droit de faire ca en fait, puisqu'ils ont pas le droit de se parler entre eux
	while (philo->rules_ptr->someone_died == 0) // j'ai pas le droit de faire ca non plus je crois
	{
		if (philo->rules_ptr->min_meals == -1 || (philo->rules_ptr->min_meals
				&& philo->nb_meals < philo->rules_ptr->min_meals))
		{
			if (!fork_pickup(philo))
				return (NULL);
			if (philo->rules_ptr->nb_of_philos == 1)
			{
				if (pthread_mutex_unlock(philo->left_fork) != 0)
					print_err("Failed to unlock mutex", 0);
				break ;
			}
			print_status("is eating", philo);
			philo->last_meal = get_timestamp(philo);
			usleep(philo->rules_ptr->t_to_eat * 1000);
			philo->nb_meals++;
//			print_status("has eaten", philo);
			has_eaten_enough(philo);
			if (!fork_putdown(philo))
				return (NULL);
			if (philo->rules_ptr->min_meals)
				if (philo->is_done_eating
					&& philo->rules_ptr->full_dinners == philo->rules_ptr->nb_of_philos)
					return (NULL);
			print_status("is sleeping", philo);
			usleep(philo->rules_ptr->t_to_sleep * 1000);
			print_status("is thinking", philo);
			usleep(1000);
//			usleep(1000);
		}
		else
			break;
	}
//	print_status("is done", philo);
//	printf("*philo %d is done\n", philo->philo_id);
	return (NULL);
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
			if (pthread_create(&rules->philos[i]->philo,
					NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
		}
		i++;
	}
	return (1);
}
