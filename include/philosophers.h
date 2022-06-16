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
# include <stdlib.h>
# include <sys/time.h>

/* ########################################################################## */
/* STRUCTURES */

typedef struct s_philo
{
	pthread_t		philo;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				philo_id;
	int				nb_meals;
	long int		last_meal;
	struct s_rules	*rules;
}				t_philo;

typedef struct s_rules
{
	long int		nb_of_philos;
	long int		die_t;
	long int		eat_t;
	long int		sleep_t;
	long int		think_t;
	long int		min_meals;
	long int		start_time;
	int				someone_died;
	int				full_dinners;
	struct s_philo	**philos;
	pthread_mutex_t	printer_m;
	pthread_mutex_t	last_meal_m;
	pthread_mutex_t	full_dinners_m;
	pthread_mutex_t	someone_died_m;
	pthread_mutex_t	philo_init;
	pthread_mutex_t	**forks;
}				t_rules;

/* ########################################################################## */
/* FUNCTIONS */

/* errors.c */
int			print_err(char *error, int return_value);
int			param_values_check(int argc, char *argv[]);
int			param_char_check(int argc, char *argv[]);

/* init.c */
void		init_struct(t_rules *rules);
int			init_and_assign_forks(t_rules *rules);

/* optimization.c */
void		fork_pickup(t_philo *philo);
void		fork_putdown(t_philo *philo);

/* philos.c */
int			launch_philos(t_rules *rules, pthread_t *monitor);
void		init_philos(t_rules *rules);

/* setup.c */
int			setup_rules(t_rules *rules, char *argv[], int argc);

/* utils.c */
void		*check_dead_philo(void *rules_tmp);
int			ft_putstr_fd(char *str, int fd);
long int	ft_atol(const char *nptr);
int			ft_is_digit(char c);
int			freester(t_rules *rules, int return_value);
long int	get_time(void);
int			print_status(char *status, t_philo *philo);
void		philo_sleep(t_philo *philo, long int time);
long int	get_timestamp(t_philo *philo);

#endif
