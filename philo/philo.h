#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
#include <sys/time.h>

typedef struct s_times
{
	size_t			start_time;
	size_t			eating_start_time;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;

}	t_times;

typedef struct s_philo
{
	int				id;
	t_times			times;
	size_t 			eaten;
	pthread_mutex_t *lfork;
    pthread_mutex_t *rfork;
	pthread_mutex_t	*write;
	pthread_mutex_t	*check_eat;
	pthread_mutex_t	*dead_mutex;
	pthread_t 		thread;
	int			*dead1;
}	t_philo;

typedef struct s_main
{
	int				philo_nb;
	int				actual_philo;
	size_t			eat_nb;
	pthread_mutex_t	write;
	pthread_mutex_t	check_eat;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t *philo_forks;
	t_philo			*philo;
	pthread_t 		checker;
	int				dead;
}	t_main;

size_t	ft_strlen(const char *s);
int	ft_isspace(int c);
int	ft_atoi(const char *str);
size_t	actual_time(void);
void	ft_usleep(size_t time_in_ms);
int	ft_strcmp(const char *s1, const char *s2);

void	init_main(t_main *main, t_philo *philo, pthread_mutex_t *philo_forks, char *argv[]);
void	init_forks(t_main *main, t_philo *philo, pthread_mutex_t *philo_forks);
void	init_values(t_main *main, t_philo *philo, pthread_mutex_t *philo_forks, char *argv[]);
void	*routine_one(void *main_p);
void	create_one_thread(t_main *main);
int	check_finish(t_philo *philo);
void	*routine(void *main_p);
void	create_threads(t_main *main);
int	philo_dead(t_main *main, int i);
int	check_death(t_main *main);
int	check_eat(t_main *main);
int	check_end(t_philo * philo);
void	destroy_and_free(t_main	*main);
void	write_status(t_philo *philo, char *action);
void	*check(void *main_p);

#endif