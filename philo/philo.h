/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamgar <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 10:00:02 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/02 10:00:04 by zamgar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	size_t			eaten;
	size_t			start_time;
	size_t			last_time_eat;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			eat_nb;
	int				philo_nb;
	int				is_eating;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	pthread_mutex_t	*write;
	pthread_mutex_t	*check_eat;
	pthread_mutex_t	*dead_mutex;
	pthread_t		thread;
	int				*dead1;
}	t_philo;

typedef struct s_main
{
	pthread_mutex_t	write;
	pthread_mutex_t	check_eat;
	pthread_mutex_t	dead_mutex;
	t_philo			*philo;
	pthread_t		checker;
	int				dead;
}	t_main;

size_t	ft_strlen(const char *s);
int		ft_isspace(int c);
int		ft_atoi(const char *str);
size_t	actual_time(void);
void	ft_usleep(size_t ms);
int		ft_strcmp(const char *s1, const char *s2);
void	write_status(t_philo *philo, char *action);

void	destroy_and_free(t_main *main, pthread_mutex_t *philo_forks);
void	create_threads(t_main *main, pthread_mutex_t *philo_forks);
void	*routine(void *philo_p);
int		check_end(t_philo *philo);
void	eat(t_philo *philo);
void	init_values(t_main *m, t_philo *ph, pthread_mutex_t *forks, char *av[]);
void	init_philo(t_main *main, t_philo *philo, char *argv[]);
void	init_forks(t_philo *philo, pthread_mutex_t *philo_forks, char *philonb);
void	init_main(t_main *main, t_philo *philo);
void	*check(void *philo_p);
int		check_eat(t_philo *philo);
int		check_death(t_philo *philo);
int		philo_dead(t_philo *philo, size_t time_to_die);

#endif
