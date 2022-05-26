/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 11:44:23 by mjallada          #+#    #+#             */
/*   Updated: 2022/05/25 11:44:28 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/* ########################################################################## */
/* INCLUDES */

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>

/* ########################################################################## */
/* STRUCTURES */

typedef struct s_philo
{
	struct s_rules	*rules;
}				t_philo;

typedef struct s_rules
{
	long int		nb_of_philos;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		min_meals;
}				t_rules;

/* ########################################################################## */
/* FUNCTIONS */

/* errors.c */
int			param_values_check(int argc, char *argv[]);
int			param_char_check(int argc, char *argv[]);

/* setup.c */
int			setup_rules(t_rules *rules, char *argv[], int argc);

/* utils.c */
void		error_printer(char *error);
int			ft_putstr_fd(char *str, int fd);
long int	ft_atol(const char *nptr);
int			ft_is_digit(char c);

#endif
