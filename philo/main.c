#include "philo.h"

void	init_values(t_main *main, int argc, char *argv[])
{
	int	i;
	int j;
	int p_nb;

	i = 0;
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
	if (main->philo_nb <= 0 || main->time_to_die <= 0 || main->time_to_eat <= 0 || main->time_to_sleep <= 0)
		exit(printf("Error\nArgument got the wrong size\n"));
	pthread_mutex_init(&main->write, NULL);
	pthread_mutex_init(&main->check_eat, NULL);
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
		pthread_mutex_init(&main->philo_forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < p_nb)
	{
		main->philo[i].id = i + 1;
		main->philo[i].eating_end_time = -1;
		main->philo[i].nb_eat = 0;
		main->philo[i].death = 0;
		i++;
	}
	i = 0;
	p_nb = p_nb - 1;
	main->philo[i].lfork = main->philo_forks[i];
	if (main->philo_nb > 1)
		main->philo[i].rfork = main->philo_forks[p_nb];
	// printf("philo %d left fork : %d\n", i + 1, i + 1);
	// printf("philo %d right fork : %d\n", i + 1, p_nb + 1);
	i += 1;
	j = 0;
	while (i < p_nb + 1)
	{
		main->philo[i].lfork = main->philo_forks[i];
		main->philo[i].rfork = main->philo_forks[j];
		// printf("philo %d left fork : %d\n", i + 1, i + 1);
		// printf("philo %d right fork : %d\n", i + 1, j + 1);
		i++;
		j++;
	}
	// printf("\n");
}

void	*routine_one(void *main_p)
{
	int	act_philo;
	t_main *main;

	main = (t_main *)main_p;
	act_philo = main->actual_philo;
	pthread_mutex_lock(&main->philo[act_philo].lfork);
	pthread_mutex_lock(&main->write);
	printf("%ld %d has taken a fork (L)\n", (actual_time() - main->start_time), act_philo + 1);
	pthread_mutex_unlock(&main->write);
	ft_usleep(main->time_to_die);
	printf("%ld %d died\n", (actual_time() - main->start_time), act_philo + 1);
	return (NULL);
}

void	create_one_thread(t_main *main)
{
	int	i;

	i = 0;
	main->actual_philo = i;
	if (pthread_create(&main->philo[i].thread, NULL, &routine_one, main) != 0)
			exit(printf("Error\nThread failed.\n"));
	if (pthread_join(main->philo[i].thread, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
		return ;
}

static int finish;

void	*routine(void *main_p)
{
	int	act_philo;
	t_main *main;

	main = (t_main *)main_p;
	act_philo = main->actual_philo;
	while (finish == 0)
	{
		pthread_mutex_lock(&main->philo[act_philo].lfork);
		pthread_mutex_lock(&main->write);
		printf("%ld %d has taken a fork (L)\n", (actual_time() - main->start_time), act_philo + 1);
		pthread_mutex_unlock(&main->write);
		pthread_mutex_lock(&main->philo[act_philo].rfork);
		pthread_mutex_lock(&main->write);
		printf("%ld %d has taken a fork (R)\n", (actual_time() - main->start_time), act_philo + 1);
		pthread_mutex_unlock(&main->write);
		pthread_mutex_lock(&main->write);
		printf("%ld %d is eating\n", (actual_time() - main->start_time), act_philo + 1);
		pthread_mutex_unlock(&main->write);
		pthread_mutex_lock(&main->check_eat);
		main->philo[act_philo].eating_end_time = actual_time() - main->start_time;
		main->philo[act_philo].nb_eat += 1;
		pthread_mutex_unlock(&main->check_eat);
		ft_usleep(main->time_to_eat);
		printf("\n%ld\n", main->philo[act_philo].nb_eat);
		pthread_mutex_unlock(&main->philo[act_philo].rfork);
		pthread_mutex_unlock(&main->philo[act_philo].lfork);
		pthread_mutex_lock(&main->write);
		printf("%ld %d is sleeping\n", (actual_time() - main->start_time), act_philo + 1);
		pthread_mutex_unlock(&main->write);
		ft_usleep(main->time_to_sleep);
		pthread_mutex_lock(&main->write);
		printf("%ld %d is thinking\n", (actual_time() - main->start_time), act_philo + 1);
		pthread_mutex_unlock(&main->write);
	}
	return (NULL);
}

void	*check_end(void *main_p)
{
	int i;
	t_main *main;
	size_t time;

	main = (t_main *)main_p;
	i = 0;
	main->check_eat_nb = 1;
	while (1)
	{
		while (i < main->philo_nb)
		{
			pthread_mutex_lock(&main->check_eat);
			time = actual_time();
			printf("end_eating_time : %ld\n", main->philo[i].eating_end_time);
			size_t ok = (time - main->start_time) - main->philo[i].eating_end_time;
			printf("ok : %ld\n", ok);
			if (ok >= main->time_to_die && main->philo[i].eating_end_time != (size_t)-1)
			{
				finish = 1;
				exit(printf("%ld %d died\n", (actual_time() - main->start_time), i + 1));
			}
			if (main->philo[i].nb_eat < main->eat_nb)
				main->check_eat_nb = 0;
			pthread_mutex_unlock(&main->check_eat);
			i++;
		}
		if (main->check_eat_nb == 1)
		{
			finish = 1;
			exit(printf("ALL EAT\n"));
		}
		i = 0;
		main->check_eat_nb = 1;
		ft_usleep(1000);
	}
	return (NULL);
}

void	create_threads(t_main *main)
{
	int	i;
	int p_nb;

	i = 0;
	p_nb = main->philo_nb;
	main->start_time = actual_time();
	// printf("start time : %ld\n", main->start_time);
	while (i < p_nb)
	{
		main->actual_philo = i;
		if (pthread_create(&main->philo[i].thread, NULL, &routine, main) != 0)
			exit(printf("Error\nThread failed.\n"));
		ft_usleep(1);
		i++;
	}
	if (pthread_create(&main->check_end, NULL, &check_end, main) != 0)
		exit(printf("Error\nThread failed.\n"));
	ft_usleep(1);
	i = 0;
	while (i < p_nb)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
		i++;
	}
	if (pthread_join(main->check_end, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
	return ;
}

void	destroy_and_free(t_main	*main)
{
	int p_nb;

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