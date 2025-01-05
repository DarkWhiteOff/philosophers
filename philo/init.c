#include "philo.h"

void	init_main(t_main *main, t_philo *philo, pthread_mutex_t *philo_forks, char *argv[])
{
	main->philo_nb = ft_atoi(argv[1]);
	if (argv[5])
		main->eat_nb = ft_atoi(argv[5]);
	else
		main->eat_nb = -1;
	main->check_eat_nb = 0;
	if (pthread_mutex_init(&main->write, NULL) != 0)
			destroy_and_free(main);
	if (pthread_mutex_init(&main->check_eat, NULL) != 0)
			destroy_and_free(main);
	main->philo_forks = philo_forks;
	main->philo = philo;
	return ;
}

void	init_forks(t_main *main, t_philo *philo, pthread_mutex_t *philo_forks)
{
	int i;
	int j;
	int p_nb;

	i = 0;
	j = 0;
	p_nb = main->philo_nb - 1;
	philo[i].lfork = &philo_forks[i];
	if (main->philo_nb > 1)
		philo[i].rfork = &philo_forks[p_nb];
	i += 1;
	while (i < p_nb + 1)
	{
		philo[i].lfork = &philo_forks[i];
		philo[i].rfork = &philo_forks[j];
		i++;
		j++;
	}
	return ;
}

void	init_values(t_main *main, t_philo *philo, pthread_mutex_t *philo_forks, char *argv[])
{
	int	i;
	int p_nb;

	i = 0;
	init_main(main, philo, philo_forks, argv);
	p_nb = main->philo_nb;
	while (i < p_nb)
	{
		philo[i].id = i + 1;
		philo[i].times.time_to_die = ft_atoi(argv[2]);
		philo[i].times.time_to_eat = ft_atoi(argv[3]);
		philo[i].times.time_to_sleep = ft_atoi(argv[4]);
		philo[i].times.start_time = actual_time();
		philo[i].times.eating_start_time = actual_time();
		philo[i].eaten = 0;
		if (pthread_mutex_init(&philo_forks[i], NULL) != 0)
			destroy_and_free(main);
		philo[i].write = &main->write;
		philo[i].check_eat = &main->check_eat;
		i++;
	}
	init_forks(main, philo, philo_forks);
	return ;
}