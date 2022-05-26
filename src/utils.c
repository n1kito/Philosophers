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

/* Prints an error message on stderr. */

void	error_printer(char *error)
{
	ft_putstr_fd("Error\n> ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(".\n", 2);
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
