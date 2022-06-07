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
	int				*somebody_died_ptr;
	struct s_rules	*rules_ptr;
}				t_philo;

typedef struct s_rules
{
	long int		nb_of_philos;
	long int		t_to_die;
	long int		t_to_eat;
	long int		t_to_sleep;
	long int		min_meals;
	long int		start_time;
	int				somebody_died; //not sure I'm using this
	int				total_meals; // not sure I'm using this either
	struct s_philo	**philos;
	pthread_mutex_t	**forks;
}				t_rules;

/* ########################################################################## */
/* FUNCTIONS */

/* errors.c */
void		print_err(char *error);
int			param_values_check(int argc, char *argv[]);
int			param_char_check(int argc, char *argv[]);

/* optimization.c */
void		fork_pickup(t_philo *philo);
void		fork_putdown(t_philo *philo);
void		stop_fork_stealing(t_philo *first_philo);
long int	get_timestamp(t_philo *philo); // TODO this needs to go to a utils file

/* philos.c */
int			launch_philos(t_rules *rules);
void		init_philos(t_rules *rules);
int			init_and_assign_forks(t_rules *rules);

/* setup.c */
void		init_struct(t_rules *rules);
int			setup_rules(t_rules *rules, char *argv[], int argc);

/* utils.c */
void		*check_dead_philo(void *rules_tmp);
int			ft_putstr_fd(char *str, int fd);
long int	ft_atol(const char *nptr);
int			ft_is_digit(char c);
void		freester(t_rules *rules);
long int	get_time(void);

#endif
