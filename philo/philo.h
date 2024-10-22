#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
	int	id;
	int	eating;
	int	sleeping;
	int	thinking;
	int	left_fork;
	int	right_fork;
	int	have_eaten;
	pthread_mutex_t lfork;
    pthread_mutex_t rfork;
	pthread_t thread;
}	t_philo;

typedef struct s_main
{
	int				actual_philo;
	struct timeval	actual_time;
	int				philo_nb;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			eat_nb;
	int				death;
	pthread_mutex_t *philo_forks;
	t_philo			*philo;
}	t_main;

size_t	ft_strlen(const char *s);
int	ft_isspace(int c);
int	ft_atoi(const char *str);

#endif