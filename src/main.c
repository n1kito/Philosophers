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
	t_rules		rules;
	pthread_t	monitor;
	int			i;

	init_struct(&rules);
	if (!setup_rules(&rules, argv, argc))
		return (freester(&rules, 1));
	if (!launch_philos(&rules, &monitor))
		return (freester(&rules, 1));
	i = 0;
	while (i < rules.nb_of_philos)
	{
		pthread_join(rules.philos[i]->philo, NULL);
		i++;
	}
//	pthread_join(monitor, NULL);
	if (!rules.someone_died && rules.min_meals
		&& rules.full_dinners == rules.nb_of_philos)
	{
		printf(OK_SYMBOL);
		printf("Everyone ate \033[1m%ld\033[0;92m time(s).\033[0;39m\n",
			rules.min_meals);
	}
	return (freester(&rules, 0));
}
