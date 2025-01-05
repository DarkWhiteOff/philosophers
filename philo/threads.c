#include "philo.h"

void	*routine_one(void *main_p)
{
	int	act_philo;
	t_main *main;

	main = (t_main *)main_p;
	act_philo = main->actual_philo;
	pthread_mutex_lock(&main->philo[act_philo].lfork);
	pthread_mutex_lock(&main->write);
	printf("%ld %d has taken a fork (L)\n", (actual_time() - main->start_time), act_philo + 1);
	pthread_mutex_unlock(&main->write);
	ft_usleep(main->time_to_die);
	printf("%ld %d died\n", (actual_time() - main->start_time), act_philo + 1);
	return (NULL);
}

void	create_one_thread(t_main *main)
{
	int	i;

	i = 0;
	main->actual_philo = i;
	main->start_time = actual_time();
	if (pthread_create(&main->philo[i].thread, NULL, &routine_one, main) != 0)
	{
		destroy_and_free(main);
		exit(printf("Error\nThread failed.\n"));
	}
	if (pthread_join(main->philo[i].thread, NULL) != 0)
	{
		destroy_and_free(main);
		exit(printf("Error\nThread join failed.\n"));
	}
	return ;
}

int	check_finish(t_main *main)
{
	pthread_mutex_lock(&main->check_dead);
	if (main->dead == 1)
		return (pthread_mutex_unlock(&main->check_dead), 1);
	pthread_mutex_unlock(&main->check_dead);
	return (0);
}

void	eat(t_main *main, int act_philo)
{
	pthread_mutex_lock(&main->philo[act_philo].lfork);
	pthread_mutex_lock(&main->write);
	printf("%ld %d has taken a fork (L)\n", (actual_time() - main->start_time), act_philo + 1);
	pthread_mutex_unlock(&main->write);
	pthread_mutex_lock(&main->philo[act_philo].rfork);
	pthread_mutex_lock(&main->write);
	printf("%ld %d has taken a fork (R)\n", (actual_time() - main->start_time), act_philo + 1);
	pthread_mutex_unlock(&main->write);
	pthread_mutex_lock(&main->write);
	printf("%ld %d is eating\n", (actual_time() - main->start_time), act_philo + 1);
	pthread_mutex_unlock(&main->write);
	pthread_mutex_lock(&main->check_eat);
	main->philo[act_philo].eating_start_time = actual_time();
	main->philo[act_philo].nb_eat += 1;
	pthread_mutex_unlock(&main->check_eat);
	ft_usleep(main->time_to_eat);
	pthread_mutex_unlock(&main->philo[act_philo].rfork);
	pthread_mutex_unlock(&main->philo[act_philo].lfork);
}

void	sim_start_delay(size_t start_time)
{
	while (actual_time() < start_time)
		continue ;
}

void	*routine(void *main_p)
{
	int	act_philo;
	t_main *main;

	main = (t_main *)main_p;
	act_philo = main->actual_philo;
	pthread_mutex_lock(&main->check_eat);
	main->philo[act_philo].eating_start_time = main->start_time;
	pthread_mutex_unlock(&main->check_eat);
	sim_start_delay(main->start_time);
	if (act_philo % 2)
		ft_usleep(1);
	while (check_finish(main) == 0)
	{
		eat(main, act_philo);
		pthread_mutex_lock(&main->write);
		printf("%ld %d is sleeping\n", (actual_time() - main->start_time), act_philo + 1);
		pthread_mutex_unlock(&main->write);
		ft_usleep(main->time_to_sleep);
		pthread_mutex_lock(&main->write);
		printf("%ld %d is thinking\n", (actual_time() - main->start_time), act_philo + 1);
		pthread_mutex_unlock(&main->write);
	}
	return (NULL);
}

void	init_threads(t_main *main)
{
	int i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	main->start_time = actual_time() + (main->philo_nb * 2 * 10);
	while (i < p_nb)
	{
		main->actual_philo = i;
		if (pthread_create(&main->philo[i].thread, NULL, &routine, main) != 0)
		{
			destroy_and_free(main);
			exit(printf("Error\nThread failed.\n"));
		}
		//ft_usleep(1);
		i++;
	}
	while (1)
	{
		if (check_death(main) == 1 || check_eat(main) == 1)
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
	
	while (i < p_nb)
	{
		main->philo[i].eating_start_time = main->start_time;
		i++;
	}
	init_threads(main);
	i = 0;
	while (i < p_nb)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
		i++;
	}
	return ;
}