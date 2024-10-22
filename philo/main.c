#include "philo.h"

int	value = 0;

void	init_values(t_main *main, int argc, char *argv[])
{
	int	i;
	int j;
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
	main->philo = 0;
	main->philo = malloc (sizeof(t_philo) * main->philo_nb);
	if (main->philo == NULL)
		return ;
	main->philo_forks = 0;
	main->philo_forks = malloc (sizeof(pthread_mutex_t) * main->philo_nb);
	if (main->philo_forks == NULL)
		return ;
	while (i < p_nb)
	{
		pthread_mutex_init(&main->philo_forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < p_nb)
	{
		main->philo[i].id = i + 1;
		main->philo[i].sleeping = 0;
		main->philo[i].eating = 0;
		main->philo[i].thinking = 0;
		main->philo[i].left_fork = 1;
		main->philo[i].have_eaten = 0;
		if (main->philo_nb > 1)
			main->philo[i].right_fork = 0;
		printf("philo %d initialised\n", i + 1);
		i++;
	}
	i = 0;
	p_nb = p_nb - 1;
	main->philo[i].lfork = main->philo_forks[i];
	if (main->philo_nb > 1)
		main->philo[i].rfork = main->philo_forks[p_nb];
	i += 1;
	j = 0;
	while (i < p_nb)
	{
		main->philo[i].lfork = main->philo_forks[i];
		main->philo[i].rfork = main->philo_forks[j];
		i++;
		j++;
	}
	printf("\n");
}

size_t	print_time(t_main *main)
{
	gettimeofday(&main->actual_time, NULL);
	return (main->actual_time.tv_usec);
}

void	*routine_one(void *main_p)
{
	int		act_philo;
	size_t	debut_time;
	size_t	finish_time;
	t_main *main;

	main = (t_main *)main_p;
	act_philo = main->actual_philo;
	pthread_mutex_lock(&main->philo[act_philo].lfork);
	debut_time = print_time(main);
	main->philo[act_philo].left_fork = 0;
	printf("%ld Philo %d has taken a fork (l)\n", print_time(main), main->philo[act_philo].id);
	finish_time = print_time(main);
	if ((finish_time - debut_time) >= main->time_to_die)
		printf("%ld Philo %d died\n", finish_time, main->philo[act_philo].id);
	return (NULL);
}

void	create_one_thread(t_main *main)
{
	int	i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	main->actual_philo = i;
	if (pthread_create(&main->philo[main->actual_philo].thread, NULL, &routine_one, main) != 0)
			exit(printf("Error\nThread failed.\n"));
	if (pthread_join(main->philo[main->actual_philo].thread, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
}

void	*routine(void *main_p)
{
	int i;
	int	act_philo;
	t_main *main;

	main = (t_main *)main_p;
	i = 0;
	act_philo = main->actual_philo;
	while (main->eat_nb > 0 || main->death == 0)
	{
		if (main->philo[act_philo].thinking == 1)
			printf("%ld Philo %d is thinking\n", print_time(main), main->philo[act_philo].id);
		pthread_mutex_lock(&main->philo[act_philo].lfork);
		main->philo[act_philo].left_fork = 0;
		printf("%ld Philo %d has taken a fork (l)\n", print_time(main), main->philo[act_philo].id);
		pthread_mutex_lock(&main->philo[act_philo].rfork);
		main->philo[act_philo].right_fork = 0;
		printf("%ld Philo %d has taken a fork (r)\n", print_time(main), main->philo[act_philo].id);
		main->philo[act_philo].thinking = 0;
		main->philo[act_philo].eating = 1;
		printf("%ld Philo %d is eating\n", print_time(main), main->philo[act_philo].id);
		usleep(main->time_to_eat);
		pthread_mutex_unlock(&main->philo[act_philo].lfork);
		pthread_mutex_unlock(&main->philo[act_philo].rfork);
		main->philo[act_philo].eating = 0;
		main->philo[act_philo].sleeping = 1;
		printf("%ld Philo %d is sleeping\n", print_time(main), main->philo[act_philo].id);
		usleep(main->time_to_sleep);
		main->philo[act_philo].sleeping = 0;
		main->philo[act_philo].thinking = 1;
		main->philo[act_philo].have_eaten = 1;
		main->eat_nb--;
	}
	return (NULL);
}

void	create_threads(t_main *main)
{
	int	i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	while (i < p_nb)
	{
		main->actual_philo = i;
		if (pthread_create(&main->philo[i].thread, NULL, &routine, main) != 0)
			exit(printf("Error\nThread failed.\n"));
		usleep(1);
		i++;
	}
	i = 0;
	while (i < p_nb)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
		i++;
	}
}

void	destroy_and_free(t_main	*main)
{
	int	i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb - 1;
	while (p_nb >= 0)
	{
		pthread_mutex_destroy(&main->philo_forks[p_nb]);
		p_nb--;
	}
	free(main->philo);
	free(main->philo_forks);
}

int main(int argc, char *argv[])
{
	t_main main;

	if (argc < 5 || argc > 6)
		return (printf("Error\nWrong numbers of arguments\n"), 1);
	init_values(&main, argc, argv);
	if (main.philo_nb == 1)
		create_one_thread(&main);
	else
		create_threads(&main);
	destroy_and_free(&main);
	return (0);
}