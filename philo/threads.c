#include "philo.h"

void	*routine_one(void *philo_p)
{
	t_philo *philo;

	philo = (t_philo *)philo_p;
	pthread_mutex_lock(philo->check_eat);
	philo->times.start_time = actual_time();
	philo->times.eating_start_time = actual_time();
	pthread_mutex_unlock(philo->check_eat);
	pthread_mutex_lock(philo->lfork);
	write_status(philo, "has taken a fork (L)");
	ft_usleep(philo->times.time_to_die);
	write_status(philo, "died");
	return (NULL);
}

void	create_one_thread(t_main *main)
{
	int	i;

	i = 0;
	if (pthread_create(&main->philo[i].thread, NULL, &routine_one, &main->philo[i]) != 0)
		destroy_and_free(main);
	if (pthread_join(main->philo[i].thread, NULL) != 0)
		destroy_and_free(main);
	return ;
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->lfork);
	write_status(philo, "has taken a fork (L)");
	pthread_mutex_lock(philo->rfork);
	write_status(philo, "has taken a fork (R)");
	write_status(philo, "is eating");
	pthread_mutex_lock(philo->check_eat);
	philo->times.eating_start_time = actual_time();
	philo->eaten++;
	pthread_mutex_unlock(philo->check_eat);
	ft_usleep(philo->times.time_to_eat);
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
}

int	check_end(t_philo * philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*(philo->dead1) == 1)
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

void	*routine(void *philo_p)
{
	t_philo *philo;

	philo = (t_philo *)philo_p;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	pthread_mutex_lock(philo->check_eat);
	philo->times.start_time = actual_time();
	philo->times.eating_start_time = actual_time();
	pthread_mutex_unlock(philo->check_eat);
	while (check_end(philo) == 0)
	{
		eat(philo);
		write_status(philo, "is sleeping");
		ft_usleep(philo->times.time_to_sleep);
		write_status(philo, "is thinking");
	}
	return (NULL);
}

void	init_threads(t_main *main)
{
	int i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	if (pthread_create(&main->checker, NULL, &check, main) != 0)
			destroy_and_free(main);
	while (i < p_nb)
	{
		if (pthread_create(&main->philo[i].thread, NULL, &routine, &main->philo[i]) != 0)
			destroy_and_free(main);

		i++;
	}
	return ;
}

void	create_threads(t_main *main)
{
	int	i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	init_threads(main);
	i = -1;
	if (pthread_join(main->checker, NULL) != 0)
			destroy_and_free(main);
	while (++i < p_nb)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			destroy_and_free(main);
	}
	return ;
}