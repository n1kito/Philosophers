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

#include "philosophers.h"

/* Prints an error message on stderr. */

int	print_err(char *error, int return_value)
{
	ft_putstr_fd("Error\n> ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(".\n", 2);
	return (return_value);
}

/* Checks for the value of all program arguments */

int	param_values_check(int argc, char *argv[])
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if (ft_atol(argv[i]) <= 0 || ft_atol(argv[i]) > INT_MAX)
		{
			ft_putstr_fd("Error\n", 2);
			break ;
		}
		i++;
	}
	if (i == argc)
		return (1);
	if (ft_atol(argv[1]) <= 0 || ft_atol(argv[1]) > INT_MAX)
		ft_putstr_fd("> [philo_count] needs a positive integer.\n", 2);
	if (ft_atol(argv[2]) <= 0 || ft_atol(argv[2]) > INT_MAX)
		ft_putstr_fd("> [time_do_die] needs a positive integer.\n", 2);
	if (ft_atol(argv[3]) <= 0 || ft_atol(argv[3]) > INT_MAX)
		ft_putstr_fd("> [eat_t] needs a positive integer.\n", 2);
	if (ft_atol(argv[4]) <= 0 || ft_atol(argv[4]) > INT_MAX)
		ft_putstr_fd("> [sleep_t] needs a positive integer.\n", 2);
	if (argc == 6 && ((ft_atol(argv[5]) <= 0) || ft_atol(argv[5]) > INT_MAX))
		ft_putstr_fd("> [minimum_meals] needs a positive integer.\n", 2);
	return (0);
}

/* Checks that there are no forbidden characters in any of the arguments.
 * The only allowed characters are digits and one '+'/'-' sign. */

int	param_char_check(int argc, char *argv[])
{
	int	i;
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
				return (print_err("Arguments contain forbidden characters", 0));
			j++;
		}
		i++;
	}
	return (1);
}
