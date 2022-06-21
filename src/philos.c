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

long int	t_since_last_meal(t_philo *philo)
{
	long int	time;

	pthread_mutex_lock(&philo->rules->last_meal_m);
	time = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->rules->last_meal_m);
	return (time);
}

/* Goes through all philos and checks if they've eaten recently enough. */

void	*check_dead_philo(void *rules_tmp)
{
	t_rules	*rules;
	int		i;

	rules = (t_rules *)rules_tmp;
	opti_sleep(rules->die_t);
//	pthread_mutex_lock(&rules->philo_init);
//	pthread_mutex_unlock(&rules->philo_init);
	i = 0;
	while (1)
	{
		if (i == rules->nb_of_philos)
		{
			if (rules->full_dinners == rules->nb_of_philos)
			{
				rules->dinner_is_over = 1;
				break ;
			}
			opti_sleep(10);
			i = 0;
		}
		if (rules->philos[i]->is_dead)
		{
			pthread_mutex_lock(&rules->someone_died_m);
			rules->someone_died = 1;
			pthread_mutex_unlock(&rules->someone_died_m);
			print_status(DEAD, rules->philos[i]);
			break ;
		}
//		if (i == rules->nb_of_philos - 1)
//			opti_sleep() // trouver combien attendre
		i++;
	}
	return (NULL);
}

//void	*check_dead_philo(void *rules_tmp)
//{
//	t_rules	*rules;
//	int		i;
//
//	rules = (t_rules *)rules_tmp;
//	pthread_mutex_lock(&rules->philo_init);
//	pthread_mutex_unlock(&rules->philo_init);
////	philo_sleep(rules->philos[0], rules->die_t);
//	opti_sleep(rules->die_t);
//	pthread_mutex_lock(&rules->full_dinners_m);
//	while (!rules->someone_died
//		&& rules->full_dinners != rules->nb_of_philos)
//	{
//		pthread_mutex_unlock(&rules->full_dinners_m);
//		i = 0;
//		while (i < rules->nb_of_philos && !rules->someone_died)
//		{
//			if (t_since_last_meal(rules->philos[i]) > rules->die_t)
//			{
//				pthread_mutex_lock(&rules->someone_died_m);
//				rules->someone_died = 1;
//				pthread_mutex_unlock(&rules->someone_died_m);
//				print_status(DEAD, rules->philos[i]);
//				return (NULL); // rajoute a la fin ptet a virer
//			}
//			i++;
//			pthread_mutex_lock(&rules->full_dinners_m);
//		}
////		philo_sleep(rules->philos[0], 8);
//		opti_sleep(100);
//		pthread_mutex_lock(&rules->full_dinners_m);
//	}
//	pthread_mutex_unlock(&rules->full_dinners_m);
//	return (NULL);
//}

/* Changes state of philo and waits for a set amount of time */

void	change_state(t_philo *philo, char *state, long int time_to_sleep)
{
	long int	time_since_last_meal;

	if (philo->is_dead)
		return ;
	print_status(state, philo);
	time_since_last_meal = t_since_last_meal(philo);
	if (time_since_last_meal + time_to_sleep > philo->rules->die_t)
	{
//		printf("\033[0;33m%d is gonna die\033[0m\n", philo->philo_id);
		opti_sleep(philo->rules->die_t - time_since_last_meal);
		philo->is_dead = 1;
	}
	else
	{
//		printf("%d has enough time to sleep\n", philo->philo_id);
		opti_sleep(time_to_sleep);
	}
//	printf("STARTS EATING\n");
}

/* Philo eating routine */

void	eating(t_philo *philo)
{
	philo->last_meal = get_time();
//	pthread_mutex_lock(&philo->rules->someone_died_m);
//	if (!philo->rules->someone_died)
//	{
//		pthread_mutex_unlock(&philo->rules->someone_died_m);
//	}
	change_state(philo, EATING, philo->rules->eat_t);
	if (philo->is_dead)
		return ;
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
//	if (philo->philo_id % 2 == 0)
//		opti_sleep(philo->rules->eat_t * 0.9);
//	pthread_mutex_lock(&philo->rules->philo_init);
//	pthread_mutex_unlock(&philo->rules->philo_init);
//	if (philo->philo_id % 2 == 0)
//		change_state(philo, THINKING, philo->rules->eat_t / 2);
//		opti_sleep(philo->rules->eat_t / 2);
//	if (philo->rules->nb_of_philos % 2 != 0 && philo->philo_id == philo->rules->nb_of_philos)
//		opti_sleep(philo->rules->eat_t);
	pthread_mutex_lock(&philo->rules->someone_died_m);
	while (!philo->is_dead && philo->rules->someone_died == 0
//		&& philo->rules->full_dinners != philo->rules->nb_of_philos)
		&& philo->nb_meals != philo->rules->min_meals)
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
//	if (philo->rules->someone_died == 0)
//		printf("\033[0;33m%d has eaten %d meals\033[0m\n", philo->philo_id, philo->nb_meals);
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
	pthread_create(monitor, NULL, &check_dead_philo, rules);
	i = 0;
	while (i < philo_count)
	{
		if ((i + 1) % 2 != 0)
			if (pthread_create(&rules->philos[i]->philo,
					NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
//		if (i % 2 == 0)
//			printf("++ Created philo %d\n", rules->philos[i]->philo_id + 1);
		i++;
	}
//	philo_sleep(rules->philos[0], rules->eat_t);
	opti_sleep(rules->eat_t);
	i = 0;
	while (i < philo_count)
	{
		if ((i + 1) % 2 == 0)
			if (pthread_create(&rules->philos[i]->philo,
					NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
////		if (i % 2 != 0)
////			printf("++ Created philo %d\n", rules->philos[i]->philo_id + 1);
		i++;
	}
//	pthread_mutex_unlock(&rules->philo_init);
	return (1);
}
