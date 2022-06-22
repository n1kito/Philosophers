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

#include "philosophers.h"

long int	t_since_last_meal(t_philo *philo)
{
	long int	time;

	pthread_mutex_lock(&philo->rules->last_meal_m);
	time = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->rules->last_meal_m);
	return (time);
}

/* Changes state of philo and checks if philo has enought time left to wait
 * for the required amount. If it does not, it dies. */

void	change_state(t_philo *philo, char *state, long int time_to_sleep)
{
	long int	time_since_last_meal;

	pthread_mutex_lock(&philo->rules->full_dinners_m);
	if (philo->is_dead
		|| philo->rules->full_dinners == philo->rules->nb_of_philos)
	{
		pthread_mutex_unlock(&philo->rules->full_dinners_m);
		return ;
	}
	pthread_mutex_unlock(&philo->rules->full_dinners_m);
	print_status(state, philo);
	time_since_last_meal = t_since_last_meal(philo);
	if (time_since_last_meal + time_to_sleep > philo->rules->die_t)
	{
		opti_sleep(philo->rules->die_t - time_since_last_meal);
		pthread_mutex_lock(&philo->rules->someone_died_m);
		philo->rules->someone_died++;
		pthread_mutex_unlock(&philo->rules->someone_died_m);
		print_status(DEAD, philo);
	}
	else
		opti_sleep(time_to_sleep);
}

/* Philo eating routine */

void	eating(t_philo *philo)
{
	philo->last_meal = get_time();
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
	if (philo->rules->nb_of_philos % 2 != 0
		&& philo->philo_id == philo->rules->nb_of_philos
		&& philo->rules->nb_of_philos != 1)
		opti_sleep(philo->rules->eat_t);
	if (philo->philo_id % 2 != 0 && philo->rules->nb_of_philos != 1)
		opti_sleep(philo->rules->eat_t);
	pthread_mutex_lock(&philo->rules->someone_died_m);
	while (!philo->is_dead && philo->rules->someone_died == 0
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
	return (NULL);
}

/* Starts philos/threads one at a time */

int	launch_philos(t_rules *rules, pthread_t *monitor)
{
	int	i;
	int	philo_count;

	philo_count = (int)rules->nb_of_philos;
	(void)monitor;
	rules->start_time = get_time();
	if (rules->start_time == -1)
		return (0);
	i = -1;
	while (++i < philo_count)
	{
		if ((i + 1) % 2 != 0)
			if (pthread_create(&rules->philos[i]->philo,
					NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
	}
	i = -1;
	while (++i < philo_count)
	{
		if ((i + 1) % 2 == 0)
			if (pthread_create(&rules->philos[i]->philo,
					NULL, &routine, rules->philos[i]) != 0)
				return (print_err("Failed to create philo", 0));
	}
	return (1);
}
