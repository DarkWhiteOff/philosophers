#include "philo.h"

void	init_main(t_main *main, int argc, char *argv[])
{
	main->philo_nb = ft_atoi(argv[1]);
	main->actual_philo = 0;
	main->time_to_die = ft_atoi(argv[2]);
	main->time_to_eat = ft_atoi(argv[3]);
	main->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		main->eat_nb = ft_atoi(argv[5]);
	else
		main->eat_nb = -1;
	main->check_eat_nb = 0;
	main->dead = 0;
	pthread_mutex_init(&main->write, NULL);
	pthread_mutex_init(&main->check_eat, NULL);
	pthread_mutex_init(&main->check_dead, NULL);
	pthread_mutex_init(&main->a_philo, NULL);
	return ;
}

void	init_forks(t_main *main)
{
	int i;
	int j;
	int p_nb;

	i = 0;
	j = 0;
	p_nb = main->philo_nb - 1;
	main->philo[i].lfork = main->philo_forks[i];
	if (main->philo_nb > 1)
		main->philo[i].rfork = main->philo_forks[p_nb];
	i += 1;
	while (i < p_nb + 1)
	{
		main->philo[i].lfork = main->philo_forks[i];
		main->philo[i].rfork = main->philo_forks[j];
		i++;
		j++;
	}
	return ;
}

void	init_values(t_main *main, int argc, char *argv[])
{
	int	i;
	int p_nb;

	i = 0;
	init_main(main, argc, argv);
	p_nb = main->philo_nb;
	main->philo = NULL;
	main->philo = malloc(sizeof(t_philo) * main->philo_nb);
	if (main->philo == NULL)
		return ;
	main->philo_forks = NULL;
	main->philo_forks = malloc(sizeof(pthread_mutex_t) * main->philo_nb);
	if (main->philo_forks == NULL)
		return ;
	while (i < p_nb)
	{
		main->philo[i].id = i + 1;
		main->philo[i].eating_start_time = 0;
		main->philo[i].nb_eat = 0;
		pthread_mutex_init(&main->philo_forks[i], NULL);
		i++;
	}
	init_forks(main);
	return ;
}