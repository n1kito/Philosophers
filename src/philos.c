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
//	pthread_mutex_lock(&rules->philo_init);
//	pthread_mutex_unlock(&rules->philo_init);
	philo_sleep(rules->philos[0], rules->die_t);
	pthread_mutex_lock(&rules->full_dinners_m);
	while (!rules->someone_died
		&& rules->full_dinners != rules->nb_of_philos)
	{
		pthread_mutex_unlock(&rules->full_dinners_m);
		i = 0;
		while (i < rules->nb_of_philos && !rules->someone_died)
		{
			pthread_mutex_lock(&rules->last_meal_m);
			if (get_timestamp(rules->philos[i])
				- rules->philos[i]->last_meal > rules->die_t)
			{
				pthread_mutex_unlock(&rules->last_meal_m);
				print_status(DEAD, rules->philos[i]);
				pthread_mutex_lock(&rules->someone_died_m);
				rules->someone_died = 1;
				pthread_mutex_unlock(&rules->someone_died_m);
			}
			else
				pthread_mutex_unlock(&rules->last_meal_m);
			i++;
		}
		pthread_mutex_lock(&rules->full_dinners_m);
	}
	pthread_mutex_unlock(&rules->full_dinners_m);
	return (NULL);
}

/* Changes state of philo and waits for a set amount of time */

void	change_state(t_philo *philo, char *state, long int time)
{
	print_status(state, philo);
	philo_sleep(philo, time);
}

/* Philo eating routine */

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->last_meal_m);
	philo->last_meal = get_timestamp(philo);
	pthread_mutex_unlock(&philo->rules->last_meal_m);
//	pthread_mutex_lock(&philo->rules->someone_died_m);
//	if (!philo->rules->someone_died)
//	{
//		pthread_mutex_unlock(&philo->rules->someone_died_m);
		change_state(philo, EATING, philo->rules->eat_t);
//	}
	philo->nb_meals++;
	if (philo->nb_meals == philo->rules->min_meals)
	{
		pthread_mutex_lock(&philo->rules->full_dinners_m);
		philo->rules->full_dinners++;
		pthread_mutex_unlock(&philo->rules->full_dinners_m);
	}
}

/* Routine that each philo has to follow */

void	*routine(void *philo_tmp)
{
	t_philo	*philo;

	philo = (t_philo *)philo_tmp;
//	if ((philo->philo_id + 1) % 2 != 0)
//	{
////		printf("0 %d is thinking\n", philo->philo_id + 1);
//		usleep(philo->rules->eat_t);
//	}
//	pthread_mutex_lock(&philo->rules->philo_init);
//	pthread_mutex_unlock(&philo->rules->philo_init);
//	if (philo->rules->nb_of_philos % 2 != 0 && philo->philo_id == 0)
//		philo_sleep(philo, philo->rules->eat_t);
	pthread_mutex_lock(&philo->rules->someone_died_m);
	while (philo->rules->someone_died == 0
		&& philo->rules->full_dinners != philo->rules->nb_of_philos)
	{
		pthread_mutex_unlock(&philo->rules->someone_died_m);
		fork_pickup(philo);
		if (philo->rules->nb_of_philos == 1)
			return (NULL);
		eating(philo);
		fork_putdown(philo);
		change_state(philo, SLEEPING, philo->rules->sleep_t);
		change_state(philo, THINKING, philo->rules->think_t);
		pthread_mutex_lock(&philo->rules->someone_died_m);
	}
	pthread_mutex_unlock(&philo->rules->someone_died_m);
	return (NULL);
}

/* Starts philos/threads one at a time */

int	launch_philos(t_rules *rules, pthread_t *monitor)
{
	int	i;
	int	philo_count;

	philo_count = (int)rules->nb_of_philos;
//	pthread_mutex_lock(&rules->philo_init);
	rules->start_time = get_time();
	if (rules->start_time == -1)
		return (0);
	i = 0;
	pthread_create(monitor, NULL, &check_dead_philo, rules);
	while (i < philo_count)
	{
		if (i % 2 == 0)
			if (pthread_create(&rules->philos[i]->philo,
					NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
//		if (i % 2 == 0)
//			printf("++ Created philo %d\n", rules->philos[i]->philo_id + 1);
		i++;
	}
	philo_sleep(rules->philos[0], rules->eat_t);
	i = 0;
	while (i < philo_count)
	{
		if (i % 2 != 0)
			if (pthread_create(&rules->philos[i]->philo,
					NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
//		if (i % 2 != 0)
//			printf("++ Created philo %d\n", rules->philos[i]->philo_id + 1);
		i++;
	}
//	pthread_mutex_unlock(&rules->philo_init);
	return (1);
}

///* Starts philos/threads one at a time */
//
//int	launch_philos(t_rules *rules, pthread_t *monitor)
//{
//	int	i;
//	int	philo_count;
//
//	philo_count = (int)rules->nb_of_philos;
//	i = 0;
//	pthread_mutex_lock(&rules->philo_init);
//	while (i < philo_count)
//	{
////		if (i % 2 == 0)
//			if (pthread_create(&rules->philos[i]->philo,
//					NULL, &routine, rules->philos[i]) != 0)
//				return (print_err("Failed to create philo", 0));
////		if (i % 2 == 0)
////			printf("++ Created philo %d\n", rules->philos[i]->philo_id + 1);
//		i++;
//	}
////	philo_sleep(rules->philos[0], 10);
////	usleep(10000);
////	i = 0;
////	while (i < philo_count)
////	{
////		if (i % 2 != 0)
////			if (pthread_create(&rules->philos[i]->philo,
////					NULL, &routine, rules->philos[i]) != 0)
////				return (print_err("Failed to create philo", 0));
////		if (i % 2 != 0)
////			printf("++ Created philo %d\n", rules->philos[i]->philo_id + 1);
////		i++;
////	}
////	usleep(10000);
//	rules->start_time = get_timestamp(rules->philos[0]);
//	pthread_create(monitor, NULL, &check_dead_philo, rules);
//	pthread_mutex_unlock(&rules->philo_init);
//	return (1);
//}
