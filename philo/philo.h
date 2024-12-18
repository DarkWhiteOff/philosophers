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

/* void	*check_end(void *main_p)
{
	int i;
	t_main *main;

	main = (t_main *)main_p;
	i = 0;
	main->check_eat_nb = 1;
	while (1)
	{
		while (i < main->philo_nb)
		{
			pthread_mutex_lock(&main->check_eat);
			//printf("end_eating_time : %ld\n", main->philo[i].eating_end_time);
			//printf("time : %ld\n", time);
			printf("difference : %ld\n", ((actual_time() - main->start_time) - main->philo[i].eating_end_time));
			if (((actual_time() - main->start_time) - main->philo[i].eating_end_time) >= main->time_to_die && main->philo[i].eating_end_time != (size_t)-1)
			{
				pthread_mutex_unlock(&main->check_eat);
				printf("%ld %d died\n", (actual_time() - main->start_time), i + 1);
				pthread_mutex_lock(&main->check_dead);
				finish = 1;
				pthread_mutex_unlock(&main->check_dead);
				return (NULL);
			}
			pthread_mutex_unlock(&main->check_eat);
			i++;
		}
		if (main->philo[i].eating_end_time != (size_t)-1)
		{
			i = 0;
			while (i < main->philo_nb)
			{
				pthread_mutex_lock(&main->check_eat);
				if (main->philo[i].nb_eat < main->eat_nb)
					main->check_eat_nb = 0;
				pthread_mutex_unlock(&main->check_eat);
				i++;
			}
			if (main->check_eat_nb == 1)
			{
				pthread_mutex_lock(&main->check_dead);
				finish = 1;
				pthread_mutex_unlock(&main->check_dead);
				printf("ALL EAT\n");
				return (NULL);
			}
		}
		i = 0;
		main->check_eat_nb = 1;
		ft_usleep(1000);
	}
	return (NULL);
} */