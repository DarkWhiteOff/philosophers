#include "philo.h"

int	philo_dead(t_main *main, int i)
{
	pthread_mutex_lock(&main->check_eat);
	if ((actual_time() - main->philo[i].times.last_time_eat) >= main->philo[i].times.time_to_die)
		return (pthread_mutex_unlock(&main->check_eat), 1);
	pthread_mutex_unlock(&main->check_eat);
	return (0);
}

int	check_death(t_main *main)
{
	int i;

	i = 0;
	while (i < main->philo_nb)
	{
		if (philo_dead(main, i) == 1)
		{
			write_status(&main->philo[i], "died");
			pthread_mutex_lock(&main->dead_mutex);
			main->dead = 1;
			pthread_mutex_unlock(&main->dead_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_eat(t_main *main)
{
	int i;
	int check_eat_nb;

	i = 0;
	check_eat_nb = 1;
	if (main->eat_nb == (size_t)-1)
		return (0);
	while (i < main->philo_nb)
	{
		pthread_mutex_lock(&main->check_eat);
		if (main->philo[i].eaten < main->eat_nb)
			check_eat_nb = 0;
		pthread_mutex_unlock(&main->check_eat);
		i++;
	}
	if (check_eat_nb == 1)
	{
		pthread_mutex_lock(&main->dead_mutex);
		main->dead = 1;
		pthread_mutex_unlock(&main->dead_mutex);
		return (1);
	}
	return (0);
}

void	*check(void *main_p)
{
	t_main *main;

	main = (t_main *)main_p;
	while (1)
	{
		if (check_death(main) == 1)
			break ;
		if (check_eat(main) == 1)
			break ;
	}
	return (NULL);
}