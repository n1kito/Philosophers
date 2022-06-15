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

// Frees memory and destroys mutexes/forks if they've been initialized.

int	freester(t_rules *rules, int return_value)
{
	int	i;

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
	i = 0;
	if (rules->forks) // TODO fix this shit
	{
		while (i < rules->nb_of_philos)
		{
			if (rules->forks[i])
			{
//				if (pthread_mutex_unlock(rules->forks[i]) != 0)
//					printf("Could not unlock mutex %d.\n", i);
//				else
//					printf("Mutex %d unlocked\n", i);
				if (pthread_mutex_destroy(rules->forks[i]) != 0)
					print_err("Could not destroy mutex", 0);
//				else
//					printf("Mutex %d destroyed.\n", i);
				free(rules->forks[i]);
			}
			i++;
		}
		free(rules->forks);
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

int	ft_strncmp(const char *first, const char *second, size_t length)
{
	size_t			i;
	unsigned char	*c1;
	unsigned char	*c2;

	i = 0;
	c1 = (unsigned char *)first;
	c2 = (unsigned char *)second;
	while (length--)
	{
		if (!c1[i] || !c2[i] || c1[i] != c2[i])
			return (c1[i] - c2[i]);
		i++;
	}
	return (0);
}

/* Protects my messages being printed and only prints if nobody died (except for
 * the message that announced the death of a philo) and if all philos are not
 * done eating. */
// Je pense que je dois mutex someone_died ici aussi

int	print_status(char *status, t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->printer_m);
	pthread_mutex_lock(&philo->rules->full_dinners_m);
	if ((philo->rules->someone_died == 0 || !ft_strncmp(status, "died", 4))
		&& philo->rules->full_dinners != philo->rules->nb_of_philos)
		printf("%ld %d %s\n", get_timestamp(philo), philo->philo_id + 1, status); // TODO replace with protected function
	pthread_mutex_unlock(&philo->rules->printer_m);
	pthread_mutex_unlock(&philo->rules->full_dinners_m);
	return (1);
}

void	philo_sleep(t_philo *philo, long int time)
{
	long int	start_time;

	start_time = get_timestamp(philo);
	while ((get_timestamp(philo) - start_time) < time && !philo->rules->someone_died)
	{
//		printf("%ld sleeping since %ld \n", get_timestamp(philo), (get_timestamp(philo) - start_time));
		usleep(600);
	}
//	printf("philo sleep done\n");
}
