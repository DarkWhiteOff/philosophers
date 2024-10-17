#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

typedef struct s_philo
{
	int	philo_id;
	int	eating;
	int	sleeping;
	int	thinking;
	int	rfork;
	int	lfork;
    pthread_t thread;
    pthread_mutex_t mutex_fork;
}	t_philo;

size_t	ft_strlen(const char *s);
int	ft_isspace(int c);
int	ft_atoi(const char *str);

#endif