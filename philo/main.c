#include "philo.h"

int	value = 0;

void	init_values(t_main *main, char *argv[])
{
	int	i;
	int p_nb;

	i = 0;
	main->philo_nb = ft_atoi(argv[1]);
	main->time_to_die = ft_atoi(argv[2]);
	main->time_to_eat = ft_atoi(argv[3]);
	main->time_to_sleep = ft_atoi(argv[4]);
	p_nb = main->philo_nb;
	main->philo = malloc (sizeof(t_philo) * main->philo_nb);
	while (p_nb > 0)
	{
		main->philo[i].philo_id = i;
		main->philo[i].sleeping = 0;
		main->philo[i].eating = 0;
		main->philo[i].thinking = 1;
		main->philo[i].rfork = 1;
		main->philo[i].lfork = 1;
		i++;
		p_nb--;
	}
}

void	*routine(void *main)
{
	printf("X is thinking\n");

	pthread_mutex_lock(&main->philo.rforks);
	pthread_mutex_lock(&main->philo.lforks);
	main->philo[0].eating = 1;
	main->philo[0].thinking = 0;
	printf("X is eating\n");
	usleep(main->time_to_eat);
	pthread_mutex_unlock(&main->philo.rforks);
	pthread_mutex_unlock(&main->philo.lforks);

	main->philo[0].sleeping = 1;
	main->philo[0].eating = 0;
	printf("X is sleeping\n");
	usleep(main->time_to_sleep);
	return ("ok");
}

void	create_threads(t_main *main)
{
	int	i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	while (p_nb > 0)
	{
		if (pthread_create(&main->philo[i].thread, NULL, &routine, main) != 0)
			exit(printf("Error\nThread creation failed.\n"));
		pthread_mutex_init(&main->philo[i].mutex_fork, NULL);
		i++;
		p_nb--;
	}
}

int main(int argc, char *argv[])
{
	t_main main;

	if (argc < 5 || argc > 6)
		return (printf("Error\nWrong numbers of arguments\n"), 0);
	init_values(&main, argv);
	create_threads(&main);
	
	return (0);
}