/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 11:44:45 by mjallada          #+#    #+#             */
/*   Updated: 2022/05/25 11:44:52 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	main(int argc, char *argv[])
{
	t_rules	rules;

	if (!setup_rules(&rules, argv, argc))
		return (1);
	printf("Rules:\n%ld philosophers\nTime to die: %ld\nTime to eat: %ld\n"
		   "Time to sleep: %ld\nMinimum meals: %ld\n",
		   rules.nb_of_philos, rules.time_to_die, rules.time_to_eat,
		   rules.time_to_sleep, rules.min_meals);
	return (0);
}
