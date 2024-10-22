#include "philo.h"

int	value = 0;

void	init_values(t_main *main, int argc, char *argv[])
{
	int	i;
	int p_nb;

	i = 0;
	main->actual_philo = 0;
	main->philo_nb = ft_atoi(argv[1]);
	main->time_to_die = ft_atoi(argv[2]);
	main->time_to_eat = ft_atoi(argv[3]);
	main->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		main->eat_nb = ft_atoi(argv[5]);
	if (main->philo_nb <= 0 || main->time_to_die < 0 || main->time_to_eat < 0 || main->time_to_sleep < 0)
		exit(printf("Error\nArgument got the wrong size\n"));
	p_nb = main->philo_nb;
	main->philo = malloc (sizeof(t_philo) * main->philo_nb);
	while (p_nb > 0)
	{
		pthread_mutex_init(&main->philo_forks[i], NULL);
		i++;
		p_nb--;
	}
	i = 0;
	p_nb = main->philo_nb;
	while (p_nb > 0)
	{
		main->philo[i].id = i;
		main->philo[i].sleeping = 0;
		main->philo[i].eating = 0;
		main->philo[i].thinking = 0;
		printf("philo %d initialised\n", i);
		i++;
		p_nb--;
	}
	printf("\n");
	/* main->philo[0].lforks = main->philo_forks[0];
	main->philo[0].rforks = main->philo_forks[4];

	main->philo[1].lforks = main->philo_forks[1];
	main->philo[1].rforks = main->philo_forks[0];

	main->philo[2].lforks = main->philo_forks[2];
	main->philo[2].rforks = main->philo_forks[1];

	main->philo[3].lforks = main->philo_forks[3];
	main->philo[3].rforks = main->philo_forks[2];

	main->philo[4].lforks = main->philo_forks[4];
	main->philo[4].rforks = main->philo_forks[3]; */ ??????
}

size_t	print_time(t_main *main)
{
	gettimeofday(&main->actual_time, NULL);
	return (main->actual_time.tv_usec);
}

void	*routine(void *main_p) // pas le tableau de philo, juste le philo simple
{
	int	p_nb;
	t_main *main;

	main = (t_main *)main_p;
	p_nb = main->actual_philo;
	if (main->philo[p_nb].thinking == 1)
		printf("%ld Philo %d is thinking\n", print_time(main), main->philo[p_nb].id);
	pthread_mutex_lock(&main->forks);
	main->philo[p_nb].thinking = 0;
	main->philo[p_nb].eating = 1;
	printf("%ld Philo %d is eating\n", print_time(main), main->philo[p_nb].id);
	usleep(main->time_to_eat);
	pthread_mutex_unlock(&main->forks);
	main->philo[p_nb].eating = 0;
	main->philo[p_nb].sleeping = 1;
	printf("%ld Philo %d is sleeping\n", print_time(main), main->philo[p_nb].id);
	usleep(main->time_to_sleep);
	main->philo[p_nb].sleeping = 0;
	main->philo[p_nb].thinking = 1;
	return (NULL);
}

void	create_threads(t_main *main)
{
	int	i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	while (p_nb > 0)
	{
		main->actual_philo = i;
		if (pthread_create(&main->philo[i].thread, NULL, &routine, main) != 0)
			exit(printf("Error\nThread creation failed.\n"));
		usleep(1);
		i++;
		p_nb--;
	}
	i = 0;
	p_nb = main->philo_nb;
	while (p_nb > 0)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
		i++;
		p_nb--;
	}
}

int main(int argc, char *argv[])
{
	t_main main;

	if (argc < 5 || argc > 6)
		return (printf("Error\nWrong numbers of arguments\n"), 1);
	init_values(&main, argc, argv);
	create_threads(&main);
	pthread_mutex_destroy(&main.forks);
	return (0);
}