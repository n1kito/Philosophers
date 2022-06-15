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
	pthread_mutex_lock(&rules->philo_init);
	pthread_mutex_unlock(&rules->philo_init);
	philo_sleep(rules->philos[0], rules->die_t);
//	printf("** %ld MONITORING STARTS\n", get_timestamp(rules->philos[0]));
	pthread_mutex_lock(&rules->full_dinners_m);
	while (!rules->someone_died
		&& rules->full_dinners != rules->nb_of_philos)
	{
		pthread_mutex_unlock(&rules->full_dinners_m);
//		philo_sleep(rules->philos[0], rules->die_t + 1);
//		if (rules->nb_of_philos > 1)
//			usleep(5);
//			usleep(rules->die_t * 1000 + 1);
		i = 0;
		while (i < rules->nb_of_philos && !rules->someone_died)
		{
//			printf("\033[0;33m**checking for a dead cunt\n\033[0m");
			pthread_mutex_lock(&rules->last_meal_m);
			if (get_timestamp(rules->philos[i])
				- rules->philos[i]->last_meal > rules->die_t)
			{
				pthread_mutex_unlock(&rules->last_meal_m);
//				printf("\033[0;31m** %ld philo %d DIED after waiting for %ld **\033[0m\n", get_time() - rules->start_time, i + 1, (get_time() - rules->start_time) - rules->philos[i]->last_meal);
//				printf("%ld %d died\n", get_timestamp(rules->philos[i]),
//					rules->philos[i]->philo_id);
				print_status("died", rules->philos[i]);
				pthread_mutex_lock(&rules->someone_died_m);
				rules->someone_died = 1;
				pthread_mutex_unlock(&rules->someone_died_m);
			}
			else
				pthread_mutex_unlock(&rules->last_meal_m);
			i++;
//			usleep(100000);
//			else
//				printf("\033[0;32m**No philo has died\n\033[0m");
		}
		pthread_mutex_lock(&rules->full_dinners_m);
	}
//	if (rules->someone_died == 1 || rules->full_dinners == rules->nb_of_philos)
//	{
//		pthread_mutex_unlock(&rules->full_dinners_m);
//		rules->dinner_is_over = 1;
//	}
	pthread_mutex_unlock(&rules->full_dinners_m);
	return (NULL);
}

/* Changes state of philo and waits for a set amount of time */

void	change_state(t_philo *philo, char *state, long int time)
{
	print_status(state, philo);
//	usleep(time);
	philo_sleep(philo, time);
}

/* Philo eating routine */
// TODO protect nb of meals variable

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->last_meal_m);
	philo->last_meal = get_timestamp(philo);
	pthread_mutex_unlock(&philo->rules->last_meal_m);
//	change_state(philo, "is eating", philo->rules->eat_t * 1000);
	change_state(philo, "is eating", philo->rules->eat_t);
//	printf("* %ld %d is done eating\n", get_timestamp(philo), philo->philo_id + 1);
	philo->nb_meals++;
	if (philo->nb_meals == philo->rules->min_meals)
	{
//		printf("%ld %d ATE ALL HIS MEALS\n", get_timestamp(philo), philo->philo_id + 1);
		pthread_mutex_lock(&philo->rules->full_dinners_m);
		philo->rules->full_dinners++;
		pthread_mutex_unlock(&philo->rules->full_dinners_m);
	}
//		philo->is_done_eating = 1;
//	}
//			print_status("has eaten", philo);
//	if (!has_eaten_enough(philo))
//		return (0);
}

/* Routine that each philo has to follow */

void	*routine(void *philo_tmp)
{
	t_philo	*philo;

	philo = (t_philo *)philo_tmp;
	pthread_mutex_lock(&philo->rules->philo_init);
	pthread_mutex_unlock(&philo->rules->philo_init);
//	if (philo->philo_id == 1)
//		philo_sleep(philo, philo->rules->sleep_t);
//	if (philo->philo_id % 2 == 0)
//		change_state(philo, "is thinking", philo->rules->think_t);
//	if (philo->philo_id % 2 != 0)
//		philo_sleep(philo, 10);
//		change_state(philo, "is sleeping", philo->rules->sleep_t);
	pthread_mutex_lock(&philo->rules->someone_died_m);
	while (philo->rules->someone_died == 0 && philo->rules->full_dinners != philo->rules->nb_of_philos)
	{
		pthread_mutex_unlock(&philo->rules->someone_died_m);
//		if (philo->rules->min_meals == -1 || (philo->rules->min_meals))
//				&& philo->nb_meals < philo->rules->min_meals))
//		{
			fork_pickup(philo);
			if (philo->rules->nb_of_philos == 1)
				return (NULL);
			eating(philo);
			fork_putdown(philo);
//			printf("%d put down the forks and can sleep\n", philo->philo_id + 1);
//			if (has_eaten_enough(philo))
//				return (NULL);
//			if (has_eaten_enough(philo))
//			{
//				printf("%ld %d IS DONE EATING\n", get_timestamp(philo), philo->philo_id + 1);
//				return (NULL);
//			}
//			change_state(philo, "is sleeping", philo->rules->sleep_t * 1000);
//			printf("%d is NOT done eating\n", philo->philo_id + 1);
			change_state(philo, "is sleeping", philo->rules->sleep_t);
//			printf("* %ld %d is done sleeping\n", get_timestamp(philo), philo->philo_id + 1);
//			change_state(philo, "is thinking", philo->rules->think_t * 1000);
			change_state(philo, "is thinking", philo->rules->think_t);
//			printf("* %ld %d is done thinking\n", get_timestamp(philo), philo->philo_id + 1);
//		}
//		else
//			break ;
		pthread_mutex_lock(&philo->rules->someone_died_m);
	}
	pthread_mutex_unlock(&philo->rules->someone_died_m);
//	printf("%ld %d IS DONE\n", get_timestamp(philo), philo->philo_id + 1);
	return (NULL);
}

/* Starts philos/threads one at a time */

int	launch_philos(t_rules *rules, pthread_t *monitor)
{
	int	i;
	int	philo_count;

	philo_count = (int)rules->nb_of_philos;
	i = 0;
//	rules->start_time = get_time(); //to remove
	pthread_mutex_lock(&rules->philo_init);
	while (i < philo_count)
	{
//		if (i % 2 == 0)
			if (pthread_create(&rules->philos[i]->philo,
					NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
//		if (i % 2 == 0)
//			printf("++ Created philo %d\n", rules->philos[i]->philo_id + 1);
		i++;
	}
//	philo_sleep(rules->philos[0], 10);
//	usleep(10000);
//	i = 0;
//	while (i < philo_count)
//	{
//		if (i % 2 != 0)
//			if (pthread_create(&rules->philos[i]->philo,
//					NULL, &routine, rules->philos[i]) != 0)
//				return (print_err("Failed to create philo", 0));
//		if (i % 2 != 0)
//			printf("++ Created philo %d\n", rules->philos[i]->philo_id + 1);
//		i++;
//	}
//	usleep(10000);
	rules->start_time = get_timestamp(rules->philos[0]);
	pthread_create(monitor, NULL, &check_dead_philo, rules);
	pthread_mutex_unlock(&rules->philo_init);
	return (1);
}
