#include "philo.h"

void	init_main(t_main *main, t_philo *philo)
{
	pthread_mutex_init(&main->write, NULL);
	pthread_mutex_init(&main->check_eat, NULL);
	pthread_mutex_init(&main->dead_mutex, NULL);
	main->dead = 0;
	main->philo = philo;
	return ;
}

void	init_forks(pthread_mutex_t *philo_forks, char *philonb)
{
	int i;

	i = 0;
	while (i < ft_atoi(philonb))
	{
		pthread_mutex_init(&philo_forks[i], NULL);
		i++;
	}
	return ;
}

void	init_philo(t_main *main, t_philo *philo, pthread_mutex_t *philo_forks, char *argv[])
{
	int i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philo[i].id = i + 1;
		philo[i].is_eating = 0;
		philo[i].eaten = 0;
		philo[i].philo_nb = ft_atoi(argv[1]);
		philo[i].time_to_die = ft_atoi(argv[2]);
		philo[i].time_to_eat = ft_atoi(argv[3]);
		philo[i].time_to_sleep = ft_atoi(argv[4]);
		if (argv[5])
			philo[i].eat_nb = ft_atoi(argv[5]);
		else
			philo[i].eat_nb = -1;
		philo[i].last_time_eat = actual_time();
		philo[i].start_time = actual_time();
		philo[i].write = &main->write;
		philo[i].check_eat = &main->check_eat;
		philo[i].dead_mutex = &main->dead_mutex;
		philo[i].dead1 = &main->dead;
		philo[i].lfork = &philo_forks[i];
		if (i == 0)
			philo[i].rfork = &philo_forks[philo[i].philo_nb - 1];
		else
			philo[i].rfork = &philo_forks[i - 1];
		i++;
	}
	return ;
}

void	init_values(t_main *main, t_philo *philo, pthread_mutex_t *philo_forks, char *argv[])
{
	init_main(main, philo);
	init_forks(philo_forks, argv[1]);
	init_philo(main, philo, philo_forks, argv);
	return ;
}