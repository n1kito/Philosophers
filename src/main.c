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

	init_struct(&rules);
	if (!setup_rules(&rules, argv, argc))
		return (freester(&rules, 1));
//	printf("%ld dinner started\n", get_time() - rules.start_time);
	pthread_create(&monitor, NULL, &check_dead_philo, &rules);
	if (!launch_philos(&rules))
		return (freester(&rules, 1));
	int i = 0;
	while (i < rules.nb_of_philos)
	{
		pthread_join(rules.philos[i]->philo, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
//	printf("***** did we get here ?\n");
	return (freester(&rules, 0));
}
