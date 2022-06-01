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

	init_struct(&rules);
	if (!setup_rules(&rules, argv, argc))
		return (freester(&rules), 1);

//	printf("Rules:\n%ld philosophers\nTime to die: %ld\nTime to eat: %ld\n"
//		"Time to sleep: %ld\nMinimum meals: %ld\n",
//		rules.nb_of_philos, rules.t_to_die, rules.t_to_eat,
//		rules.t_to_sleep, rules.min_meals);

//	long int time;
//	time = get_time();
//	printf("time: %ld\n", time);
//	sleep(2);
//	printf("You waited %d seconds.\n", (int)((get_time() - time) / 1000));
//	return (freester(&rules), 0);
	printf("%ld dinner started\n", get_time() - rules.start_time);
	pthread_t monitor;
	pthread_create(&monitor, NULL, &check_dead_philo, &rules);
	if (!launch_philos(&rules))
		return (freester(&rules), 1);
}
