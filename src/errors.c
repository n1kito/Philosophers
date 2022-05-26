/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 17:18:53 by mjallada          #+#    #+#             */
/*   Updated: 2022/05/26 17:19:18 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/* Prints an error message on stderr. */

void	error_printer(char *error)
{
	ft_putstr_fd("Error\n> ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(".\n", 2);
}

/* Checks for the value of all program arguments */
// TODO: I don't know if all these error conditions are accurate

int	param_values_check(int argc, char *argv[])
{
	int 	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atol(argv[i]) <= 0)
		{
			ft_putstr_fd("Error\n", 2);
			break;
		}
		i++;
	}
	if (i == argc)
		return (1);
	if (ft_atol(argv[1]) <= 0)
		ft_putstr_fd("> ./philo needs at least one philosopher.\n", 2);
	if (ft_atol(argv[2]) <= 0)
		ft_putstr_fd("> [time_do_die] can't be negative or 0.\n", 2);
	if (ft_atol(argv[3]) <= 0)
		ft_putstr_fd("> [time_to_eat] can't be negative or 0.\n", 2);
	if (ft_atol(argv[4]) <= 0)
		ft_putstr_fd("> [time_to_sleep] can't be negative or 0.\n", 2);
	if (argc == 6 && ft_atol(argv[5]) <= 0)
		ft_putstr_fd("> Philosophers need to eat at least once.\n", 2); // Todo: check this
	return (0);
}

/* Checks that there are no forbidden characters in any of the arguments.
 * The only allowed characters are digits and one '+'/'-' sign. */

int	param_char_check(int argc, char *argv[])
{
	int i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		if (argv[i][j] == '+' || argv[i][j] == '-')
			j++;
		while (argv[i][j])
		{
			if (!ft_is_digit(argv[i][j]))
				return (error_printer("Arguments contain forbidden characters"), 0);
			j ++;
		}
		i++;
	}
	return (1);
}
