#include "philo.h"

int	value = 0;

void	init_values(t_main *main, char *argv[])
{
	int	i;
	int p_nb;

	i = 0;
	main->actual_philo = 0;
	main->philo_nb = ft_atoi(argv[1]);
	main->time_to_die = ft_atoi(argv[2]);
	main->time_to_eat = ft_atoi(argv[3]);
	main->time_to_sleep = ft_atoi(argv[4]);
	p_nb = main->philo_nb;
	main->philo = malloc (sizeof(t_philo) * main->philo_nb);
	while (p_nb > 0)
	{
		main->philo[i].id = i;
		main->philo[i].sleeping = 0;
		main->philo[i].eating = 0;
		main->philo[i].thinking = 1;
		printf("philo %d initialised\n", i);
		i++;
		p_nb--;
	}
	printf("\n");
}

void	*routine(t_main *main) // pas le tableau de philo, juste le philo simple
{
	int	p_nb;

	p_nb = main->actual_philo;
	if (main->philo[p_nb].thinking == 1)
		printf("Philo %d is thinking\n", main->philo[p_nb].id);
	pthread_mutex_lock(&main->philo[p_nb].rfork);
	pthread_mutex_lock(&main->philo[p_nb].lfork);
	main->philo[p_nb].eating = 1;
	main->philo[p_nb].thinking = 0;
	printf("Philo %d is eating\n", main->philo[p_nb].id);
	usleep(main->philo[p_nb].time_to_eat);
	pthread_mutex_unlock(&main->philo[p_nb].rfork);
	pthread_mutex_unlock(&main->philo[p_nb].lfork);
	main->philo[p_nb].sleeping = 1;
	main->philo[p_nb].eating = 0;
	printf("Philo %d is sleeping\n", main->philo[p_nb].id);
	usleep(main->philo[p_nb].time_to_sleep);
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
		pthread_mutex_init(&main->philo[i].rfork, NULL);
		pthread_mutex_init(&main->philo[i].lfork, NULL);
		main->actual_philo = i;
		if (pthread_create(&main->philo[i].thread, NULL, &routine, main) != 0)
			exit(printf("Error\nThread creation failed.\n"));
		i++;
		p_nb--;
	}
	i = 0;
	p_nb = main->philo_nb;
	while (p_nb > 0)
	{
		if (pthread_join(&main->philo[i].thread, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
		i++;
		p_nb--;
	}
	p_nb = main->philo_nb;
	while (p_nb > 0)
	{
		pthread_mutex_destroy(&main->philo[p_nb].rfork);
		pthread_mutex_destroy(&main->philo[p_nb].lfork);
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