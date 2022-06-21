/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 10:08:19 by mjallada          #+#    #+#             */
/*   Updated: 2022/05/26 10:08:32 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/* Allocates memory to my structures, also initializing my philos to NULL to
 * avoid segfaults if the free function is called */

int	mem_alloc(t_rules *rules)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t *) * rules->nb_of_philos);
	if (rules->forks == NULL)
		return (print_err("Could not allocate memory for fork pointers", 0));
	rules->philos = malloc(sizeof(t_philo *) * rules->nb_of_philos);
	if (rules->philos == NULL)
		return (print_err("Could not allocate memory for philos", 0));
	i = 0;
	while (i < rules->nb_of_philos)
	{
		rules->forks[i] = malloc(sizeof(pthread_mutex_t) * rules->nb_of_philos);
		if (rules->forks[i] == NULL)
			return (print_err("Could not allocate memory for fork", 0));
		i++;
	}
	i = 0;
	while (i < rules->nb_of_philos)
		rules->philos[i++] = NULL;
	i = 0;
	while (i < rules->nb_of_philos)
	{
		rules->philos[i] = malloc(sizeof(t_philo) * rules->nb_of_philos);
		if (rules->philos[i] == NULL)
			return (print_err("Could not allocate memory for philo", 0));
		i++;
	}
	return (1);
}

/* Assigns parameter variables and allocates memories for the forks, philosopher
 * array and individual philosophers. Each philosopher pointer is set to NULL
 * before calling malloc() so I don't try to free an uninitialized value in case
 * of error. */

static int	init_rules(char *argv[], int argc, t_rules *rules)
{
	rules->nb_of_philos = ft_atol(argv[1]);
	rules->die_t = ft_atol(argv[2]);
	rules->eat_t = ft_atol(argv[3]);
	rules->sleep_t = ft_atol(argv[4]);
	if (rules->eat_t <= rules->sleep_t)
		rules->think_t = 1;
	else
		rules->think_t = rules->eat_t - rules->sleep_t;
//	rules->start_time = get_time(); // moved to launch philos function
	rules->someone_died = 0;
	rules->full_dinners = 0;
	rules->dinner_is_over = 0;
//	if (rules->start_time == -1)
//		return (0);
	if (argc == 6)
		rules->min_meals = ft_atol(argv[5]);
	if (pthread_mutex_init(&rules->printer_m, NULL) != 0)
		return (print_err("Failed to initiate printer_m mutex", 0));
	if (pthread_mutex_init(&rules->last_meal_m, NULL) != 0)
		return (print_err("Failed to initiate last_meal_m mutex", 0));
	if (pthread_mutex_init(&rules->full_dinners_m, NULL) != 0)
		return (print_err("Failed to initiate full_dinners_m mutex", 0));
	if (pthread_mutex_init(&rules->someone_died_m, NULL) != 0)
		return (print_err("Failed to initiate someone_died_m mutex", 0));
	if (pthread_mutex_init(&rules->philo_init, NULL) != 0)
		return (print_err("Failed to initiate someone_died_m mutex", 0));
	return (1);
}

/* Sets struct elements to NULL, helps avoid segfaults in freeing function */

void	init_struct(t_rules *rules)
{
	rules->philos = NULL;
	rules->forks = NULL;
	rules->min_meals = -1;
}

int	setup_rules(t_rules *rules, char *argv[], int argc)
{
	if (argc != 5 && argc != 6)
		return (print_err("./philo needs 4 or 5 arguments", 0));
	if (!param_char_check(argc, argv) || !param_values_check(argc, argv)
		|| !init_rules(argv, argc, rules) || !mem_alloc(rules)
		|| !init_and_assign_forks(rules))
		return (0);
	init_philos(rules);
	return (1);
}
