#include "philo.h"

void	destroy_and_free(t_main	*main)
{
	int p_nb;

	pthread_mutex_destroy(&main->write);
	pthread_mutex_destroy(&main->check_eat);
	pthread_mutex_destroy(&main->check_dead);
	pthread_mutex_destroy(&main->a_philo);
	if (main->philo)
	{
		p_nb = main->philo_nb - 1;
		while (p_nb >= 0)
		{
			pthread_mutex_destroy(&main->philo_forks[p_nb]);
			p_nb--;
		}
		free(main->philo);
	}
	if (main->philo_forks)
		free(main->philo_forks);
	exit(1);
}

int main(int argc, char *argv[])
{
	t_main main;

	if (argc < 5 || argc > 6)
		return (printf("Error\nWrong numbers of arguments\n"), 1);
	if (ft_atoi(argv[1]) <= 0)
		exit(printf("Error\nWrong arguments\n"));
	if (ft_atoi(argv[2]) <= 0 || ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0)
		exit(printf("Error\nWrong arguments\n"));
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) < 0)
			exit(printf("Error\nWrong arguments\n"));
	}
	init_values(&main, argc, argv);
	if (main.philo_nb == 1)
		create_one_thread(&main);
	else
		create_threads(&main);
	destroy_and_free(&main);
	return (0);
}