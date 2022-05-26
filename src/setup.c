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

void	rules_init(char *argv[], int argc, t_rules *rules)
{
	rules->nb_of_philos = ft_atol(argv[1]);
	rules->time_to_die = ft_atol(argv[2]);
	rules->time_to_eat = ft_atol(argv[3]);
	rules->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		rules->min_meals = ft_atol(argv[5]);
	else
		rules->min_meals = -1;
}

int	setup_rules(t_rules *rules, char *argv[], int argc)
{
	if (argc != 5 && argc != 6)
		return (error_printer("./philo needs 4 or 5 arguments"), 0);
	if (!param_char_check(argc, argv) || !param_values_check(argc, argv))
		return (0);
	rules_init(argv, argc, rules);
	return (1);
}
