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

static int	argument_check(t_rules *rules, char *argv[], int argc)
{
	(void)rules;
	(void)argc;
	(void)argv;
	if (argc < 5)
		return (error_printer("./philo needs more arguments"), 0);
//	if (ft_atol(argv[1]) <= 0 || ft_atol(argv[1]) <= 0)
	return (1);
}

int	setup_rules(t_rules *rules, char *argv[], int argc)
{
	if (!argument_check(rules, argv, argc))
		return (0);
	return (1);
}
