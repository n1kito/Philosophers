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

/* Check dead philo */

void	*check_dead_philo(void *rules_tmp)
{
	t_rules	*rules;

	rules = (t_rules *)rules_tmp;
	int	i;

	while (1)
	{
		i = 0;
		while (i < rules->nb_of_philos)
		{
//			printf("\033[0;33m**checking for a dead cunt\n\033[0m");
			if ((get_time() - rules->start_time) - rules->philos[i]->last_meal > rules->t_to_die)
			{
				printf("\033[0;31m** %ld philo %d DIED after waiting for %ld **\033[0m\n", get_time() - rules->start_time, rules->philos[i]->philo_id, (get_time() - rules->start_time) - rules->philos[i]->last_meal);
				exit (1);
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
		printf("\033[0;32mDINNER'S OVER FUCKERS\033[0m ");
		printf("(%d servings each)\n", (int)rules->min_meals);
		exit (0); // I need to free shit here.
	}
}

/* Routine that each philo has to follow */

void	*routine(void *philo_tmp)
{
	t_philo	*philo;

	philo = (t_philo *)philo_tmp;
//	int i = 5;
	if (philo->philo_id == 0 && philo->nb_meals) // tentative pour eviter au dernier de crever si le premier qui a deja mange lui pique sa fourchette
	{
		while (philo->rules_ptr->philos[4]->nb_meals != philo->nb_meals)
			usleep(1);
	}
	while (1)
	{
		if (philo->rules_ptr->min_meals == -1 || (philo->rules_ptr->min_meals && philo->nb_meals < philo->rules_ptr->min_meals))
		{
			if (philo->philo_id % 2 == 0)
			{
				if (pthread_mutex_lock(philo->left_fork) == 0)
					printf("%ld %d has taken a fork (left)\n",
//				printf("%ld %d has taken a fork\n",
						   get_time() - philo->rules_ptr->start_time,
						   philo->philo_id);
				if (philo->right_fork && pthread_mutex_lock(philo->right_fork) == 0)
					printf("%ld %d has taken a fork (right)\n",
//				printf("%ld %d has taken a fork\n",
						   get_time() - philo->rules_ptr->start_time,
						   philo->philo_id);
			}
			else
			{
				if (philo->right_fork && pthread_mutex_lock(philo->right_fork) == 0)
					printf("%ld %d has taken a fork (right)\n",
//				printf("%ld %d has taken a fork\n",
						   get_time() - philo->rules_ptr->start_time,
						   philo->philo_id);
				if (pthread_mutex_lock(philo->left_fork) == 0)
					printf("%ld %d has taken a fork (left)\n",
//				printf("%ld %d has taken a fork\n",
						   get_time() - philo->rules_ptr->start_time,
						   philo->philo_id);
			}
			printf("%ld %d is eating\n",
				   get_time() - philo->rules_ptr->start_time, philo->philo_id);
			philo->last_meal = get_time();
			usleep(philo->rules_ptr->t_to_eat * 1000);
			philo->nb_meals++;
			printf("philo %d has eaten\n", philo->philo_id);
			if (philo->rules_ptr->min_meals)
				check_number_of_meals(philo->rules_ptr);
			if (philo->philo_id % 2 == 0)
			{
				pthread_mutex_unlock(
						philo->left_fork); // check return here see above
				printf("%d unlocked left fork\n", philo->philo_id);
				pthread_mutex_unlock(philo->right_fork); // check return
				printf("%d unlocked right fork\n", philo->philo_id);
			}
			else
			{
				pthread_mutex_unlock(philo->right_fork); // check return
				printf("%d unlocked right fork\n", philo->philo_id);
				pthread_mutex_unlock(
						philo->left_fork); // check return here see above
				printf("%d unlocked left fork\n", philo->philo_id);
			}
			printf("%ld %d is thinking\n",
				   get_time() - philo->rules_ptr->start_time, philo->philo_id);
			printf("%ld %d is sleeping\n",
				   get_time() - philo->rules_ptr->start_time, philo->philo_id);
			usleep(philo->rules_ptr->t_to_sleep * 1000);
		}
	}
	return (0);
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
			printf("%ld Initiate even philo %d\n", get_time() - rules->start_time, i);
			if (pthread_create(&rules->philos[i]->philo, NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo"), 0);
		}
		i++;
	}
	i = 0;
	while (i < philo_count)
	{
		if (i % 2 != 0)
		{
			printf("%ld Initiate odd philo %d\n", get_time() - rules->start_time, i);
			if (pthread_create(&rules->philos[i]->philo, NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo"), 0);
		}
		i++;
	}
	i = 0;
	while (i < rules->nb_of_philos)
		pthread_join(rules->philos[i++]->philo, NULL);
	return (1);
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
//		rules->philos[i]->philo_id = i + 1; // I added +1 so my philos had the correct id
		rules->philos[i]->philo_id = i;
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
			return (print_err("Failed to initiate mutex"), 0);
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
