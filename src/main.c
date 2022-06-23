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

#include "philosophers.h"

int	main(int argc, char *argv[])
{
	t_rules		rules;
	int			i;
	int			return_code;

	return_code = 0;
	init_struct(&rules);
	if (!setup_rules(&rules, argv, argc))
		return (freester(&rules, 1));
	return_code = launch_philos(&rules);
	i = -1;
	while (++i < rules.nb_of_philos)
		if (pthread_join(rules.philos[i]->philo, NULL))
			return_code = simulation_error(&rules, "join");
	if (!rules.someone_died && rules.min_meals
		&& rules.full_dinners == rules.nb_of_philos)
	{
		printf(OK_SYMBOL);
		printf("Everyone ate \033[1m%ld\033[0;92m time(s).\033[0;39m\n",
			rules.min_meals);
	}
	return (freester(&rules, return_code));
}
