#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
	int				id;
	size_t			eating_end_time;
	size_t 			nb_eat;
	pthread_mutex_t lfork;
    pthread_mutex_t rfork; // voir si un des deux mutex devient un pointeur
	pthread_t 		thread;
	int				death;
}	t_philo;

typedef struct s_main
{
	int				philo_nb;
	int				actual_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			eat_nb;
	int				check_eat_nb;
	pthread_mutex_t	write;
	pthread_mutex_t	check_eat;
	pthread_mutex_t *philo_forks;
	pthread_t		check_end;
	t_philo			*philo;
	size_t			start_time;
}	t_main;

size_t	ft_strlen(const char *s);
int	ft_isspace(int c);
int	ft_atoi(const char *str);
size_t	actual_time(void);
void	ft_usleep(size_t time_in_ms);

#endif