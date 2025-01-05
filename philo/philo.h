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
	size_t			eating_start_time;
	size_t 			nb_eat;
	pthread_mutex_t lfork;
    pthread_mutex_t rfork; // voir si un des deux mutex devient un pointeur
	pthread_t 		thread;
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
	int				dead;
	pthread_mutex_t	write;
	pthread_mutex_t	check_eat;
	pthread_mutex_t	check_dead;
	pthread_mutex_t	a_philo;
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

void	init_main(t_main *main, int argc, char *argv[]);
void	init_forks(t_main *main);
void	init_values(t_main *main, int argc, char *argv[]);
void	*routine_one(void *main_p);
void	create_one_thread(t_main *main);
int	check_finish(t_main *main);
void	*routine(void *main_p);
void	create_threads(t_main *main);
int	philo_dead(t_main *main, int i);
int	check_death(t_main *main);
int	check_eat(t_main *main);
void	*check_end(void *main_p);
void	destroy_and_free(t_main	*main);

#endif