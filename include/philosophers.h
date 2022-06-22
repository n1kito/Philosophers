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

# ifndef BORING
#  define FORK "\U0001F374 has taken a fork"
#  define EATING "\U0001F35D \033[0;33mis eating\033[0m"
#  define SLEEPING "\U0001F4A4 \033[0;34mis sleeping\033[0m"
#  define THINKING "\U0001F4AD \033[0;35mis thinking\033[0m"
#  define DEAD "\U0000274C \033[1;31mdied\033[0m"
#  define STATUS "\x1b[1m%ld\x1b[0m\t[%d]\t%s\n"
#  define OK_SYMBOL "\U0001F60A \033[0;92m "
# else
#  define FORK "has taken a fork"
#  define EATING "is eating"
#  define SLEEPING "is sleeping"
#  define THINKING "is thinking"
#  define DEAD "\033[1;31mdied\033[0m"
#  define STATUS "%ld %d %s\n"
#  define OK_SYMBOL "\033[0;92m"
# endif

# define INT_MAX 2147483647

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
	int				is_dead;
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
	int				initialized_mutexes;
	struct s_philo	**philos;
	pthread_mutex_t	printer_m;
	pthread_mutex_t	last_meal_m;
	pthread_mutex_t	full_dinners_m;
	pthread_mutex_t	someone_died_m;
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
int			ft_putstr_fd(char *str, int fd);
long int	ft_atol(const char *nptr);
int			ft_is_digit(char c);
long int	get_time(void);
char		*ft_strstr(char *str, char *to_find);

/* utils_philos.c */
int			freester(t_rules *rules, int return_value);
int			print_status(char *status, t_philo *philo);
void		opti_sleep(long int time);
long int	get_timestamp(t_philo *philo);

#endif
