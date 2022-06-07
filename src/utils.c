/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 11:43:10 by mjallada          #+#    #+#             */
/*   Updated: 2022/05/26 11:43:28 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	freester(t_rules *rules, int return_value)
{
	int	i;

	if (rules->forks)
		free(rules->forks);
	i = 0;
	if (rules->philos)
	{
		while (i < rules->nb_of_philos)
		{
			if (rules->philos[i])
				free(rules->philos[i]);
			i++;
		}
		free(rules->philos);
	}
	return (return_value);
}

int	ft_putstr_fd(char *str, int fd)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
		write(fd, &str[i++], 1);
	return (i);
}

long int	ft_atol(const char *nptr)
{
	long int	num;
	int			sign;
	int			i;

	num = 0;
	sign = 1;
	i = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		num = (num * 10) + (nptr[i] - 48);
		i++;
	}
	return (num * sign);
}

int	ft_is_digit(char c)
{
	if (c < '0' || c > '9')
		return (0);
	return (1);
}

/* Converts the values of gettimeofday() to milliseconds */

long int	get_time(void)
{
	long int		time;
	struct timeval	time_struct;

	if (gettimeofday(&time_struct, NULL) == -1)
		return (print_err("Could not get time of day", -1));
	time = (long int)time_struct.tv_sec * 1000
		+ (long int)time_struct.tv_usec / 1000;
	return (time);
}
