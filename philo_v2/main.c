#include "philo.h"

void	destroy_and_free(t_main *main, pthread_mutex_t *philo_forks)
{
	int i;

	i = 0;
	pthread_mutex_destroy(&main->write);
	pthread_mutex_destroy(&main->check_eat);
	pthread_mutex_destroy(&main->dead_mutex);
	while (i < main->philo[0].philo_nb)
	{
		pthread_mutex_destroy(&philo_forks[i]);
		i++;
	}
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
	create_threads(&main, philo_forks);
	destroy_and_free(&main, philo_forks);
	return (0);
}