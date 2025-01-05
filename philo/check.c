#include "philo.h"

int	philo_dead(t_main *main, int i)
{
	pthread_mutex_lock(&main->check_eat);
	if ((actual_time() - main->philo[i].times.eating_start_time) >= main->philo[i].times.time_to_die && main->eat_nb == (size_t)-1)
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
			pthread_mutex_lock(&main->write);
			printf("%ld %d died\n", (actual_time() - main->philo[i].times.eating_start_time), main->philo[i].id);
			//pthread_mutex_unlock(&main->write);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_eat(t_main *main)
{
	int i;

	i = 0;
	main->check_eat_nb = 1;
	if (main->eat_nb == (size_t)-1)
		return (0);
	while (i < main->philo_nb)
	{
		pthread_mutex_lock(&main->check_eat);
		if (main->philo[i].eaten < main->eat_nb)
			main->check_eat_nb = 0;
		pthread_mutex_unlock(&main->check_eat);
		i++;
	}
	if (main->check_eat_nb == 1)
	{
		pthread_mutex_lock(&main->write);
		printf("ALL EAT\n");
		//pthread_mutex_unlock(&main->write);
		return (1);
	}
	return (0);
}