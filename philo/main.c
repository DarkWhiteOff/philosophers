#include "philo.h"

void	destroy_and_free(t_main	*main)
{
	int p_nb;

	p_nb = main->philo_nb;
	while (--p_nb >= 0)
		pthread_mutex_destroy(&main->philo_forks[p_nb]);
	pthread_mutex_destroy(&main->write);
	pthread_mutex_destroy(&main->check_eat);
	exit(1);
}

int main(int argc, char *argv[])
{
	t_main			main;
	t_philo			philo[200];
	pthread_mutex_t	philo_forks[200];

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
	init_values(&main, philo, philo_forks, argv);
	if (main.philo_nb == 1)
		create_one_thread(&main);
	else
		create_threads(&main);
	destroy_and_free(&main);
	return (0);
}