#include "philo.h"

void	*routine_one(void *philo_p)
{
	t_philo *philo;

	philo = (t_philo *)philo_p;
	pthread_mutex_lock(philo->check_eat);
	philo->times.eating_start_time = actual_time();
	pthread_mutex_unlock(philo->check_eat);
	pthread_mutex_lock(philo->lfork);
	pthread_mutex_lock(philo->write);
	printf("%ld %d has taken a fork (L)\n", (actual_time() - philo->times.start_time), philo->id);
	pthread_mutex_unlock(philo->write);
	ft_usleep(philo->times.time_to_die);
	printf("%ld %d died\n", (actual_time() - philo->times.start_time), philo->id);
	return (NULL);
}

void	create_one_thread(t_main *main)
{
	int	i;

	i = 0;
	if (pthread_create(&main->philo[i].thread, NULL, &routine_one, &main->philo[i]) != 0)
	{
		//printf("Create failed\n");
		destroy_and_free(main);
	}
	if (pthread_join(main->philo[i].thread, NULL) != 0)
	{
		//printf("Join failed\n");
		destroy_and_free(main);
	}
	return ;
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->lfork);
	pthread_mutex_lock(philo->write);
	printf("%ld %d has taken a fork (L)\n", (actual_time() - philo->times.start_time), philo->id);
	pthread_mutex_unlock(philo->write);
	pthread_mutex_lock(philo->rfork);
	pthread_mutex_lock(philo->write);
	printf("%ld %d has taken a fork (R)\n", (actual_time() - philo->times.start_time), philo->id);
	pthread_mutex_unlock(philo->write);
	pthread_mutex_lock(philo->check_eat);
	pthread_mutex_lock(philo->write);
	printf("%ld %d is eating\n", (actual_time() - philo->times.start_time), philo->id);
	pthread_mutex_unlock(philo->write);
	philo->times.eating_start_time = actual_time();
	philo->eaten += 1;
	pthread_mutex_unlock(philo->check_eat);
	ft_usleep(philo->times.time_to_eat);
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
}

void	*routine(void *philo_p)
{
	t_philo *philo;

	philo = (t_philo *)philo_p;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	pthread_mutex_lock(philo->check_eat);
	philo->times.eating_start_time = actual_time();
	pthread_mutex_unlock(philo->check_eat);
	while (1)
	{
		eat(philo);
		pthread_mutex_lock(philo->write);
		printf("%ld %d is sleeping\n", (actual_time() - philo->times.start_time), philo->id);
		pthread_mutex_unlock(philo->write);
		ft_usleep(philo->times.time_to_sleep);
		pthread_mutex_lock(philo->write);
		printf("%ld %d is thinking\n", (actual_time() - philo->times.start_time), philo->id);
		pthread_mutex_unlock(philo->write);
	}
	return (NULL);
}

void	init_threads(t_main *main)
{
	int i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	while (i < p_nb)
	{
		if (pthread_create(&main->philo[i].thread, NULL, &routine, &main->philo[i]) != 0)
		{
			//printf("Create failed\n");
			destroy_and_free(main);
		}
		i++;
	}
	while (1)
	{
		if (check_death(main) == 1)
			return ;
		if (check_eat(main) == 1)
			return ;
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
	while (++i < p_nb)
	{
		if (pthread_detach(main->philo[i].thread) != 0)
		{
			//printf("Detach failed\n");
			destroy_and_free(main);
		}
	}
	return ;
}