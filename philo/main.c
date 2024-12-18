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
	main->dead = 0;
	if (main->philo_nb <= 0 || main->time_to_die <= 0 || main->time_to_eat <= 0 || main->time_to_sleep <= 0)
		exit(printf("Error\nArgument got the wrong size\n"));
	pthread_mutex_init(&main->write, NULL);
	pthread_mutex_init(&main->check_eat, NULL);
	pthread_mutex_init(&main->check_dead, NULL);
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

int	check_finish(t_main *main)
{
	pthread_mutex_lock(&main->check_dead);
	if (main->dead == 1)
		return (pthread_mutex_unlock(&main->check_dead), 1);
	pthread_mutex_unlock(&main->check_dead);
	return (0);
}

void	*routine(void *main_p)
{
	int	act_philo;
	t_main *main;

	main = (t_main *)main_p;
	act_philo = main->actual_philo;
	if (act_philo % 2 == 0)
		ft_usleep(1);
	while (check_finish(main) == 0)
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
		//if (act_philo == 2)
		//	printf("eating debut time : %ld\n", main->philo[act_philo].eating_end_time);
		main->philo[act_philo].nb_eat += 1;
		pthread_mutex_unlock(&main->check_eat);
		ft_usleep(main->time_to_eat);
		//printf("\n%ld\n", main->philo[act_philo].nb_eat);
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

int	philo_dead(t_main *main, int i)
{
	pthread_mutex_lock(&main->check_eat);
	if (((actual_time() - main->start_time) - main->philo[i].eating_end_time) >= main->time_to_die && main->philo[i].eating_end_time != (size_t)-1)
		return (pthread_mutex_unlock(&main->check_eat), 1);
	pthread_mutex_unlock(&main->check_eat);
	return (0);
}

int	check_death(t_main *main)
{
	int i;

	i = 0;
	while (i < main->philo_nb)
	{
		if (philo_dead(main, i) == 1)
		{
			pthread_mutex_lock(&main->write);
			printf("%ld %d died\n", (actual_time() - main->start_time), i + 1);
			pthread_mutex_unlock(&main->write);
			pthread_mutex_lock(&main->check_dead);
			main->dead = 1;
			pthread_mutex_unlock(&main->check_dead);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_eat(t_main *main)
{
	int i;

	i = 0;
	main->check_eat_nb = 1;
	if (main->eat_nb == (size_t)-1)
		return (0);
	i = 0;
	while (i < main->philo_nb)
	{
		pthread_mutex_lock(&main->check_eat);
		if (main->philo[i].nb_eat < main->eat_nb)
			main->check_eat_nb = 0;
		pthread_mutex_unlock(&main->check_eat);
		i++;
	}
	if (main->check_eat_nb == 1)
	{
		pthread_mutex_lock(&main->check_dead);
		main->dead = 1;
		pthread_mutex_unlock(&main->check_dead);
		pthread_mutex_lock(&main->write);
		printf("ALL EAT\n");
		pthread_mutex_unlock(&main->write);
		return (1);
	}
	return (0);
}

void	*check_end(void *main_p)
{
	t_main *main;

	main = (t_main *)main_p;
	while (1)
	{
		if (check_death(main) == 1 || check_eat(main) == 1)
			return (NULL);
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
	if (pthread_create(&main->check_end, NULL, &check_end, main) != 0)
		exit(printf("Error\nThread failed.\n"));
	while (i < p_nb)
	{
		main->actual_philo = i;
		if (pthread_create(&main->philo[i].thread, NULL, &routine, main) != 0)
			exit(printf("Error\nThread failed.\n"));
		i++;
	}
	i = 0;
	if (pthread_join(main->check_end, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
	while (i < p_nb)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			exit(printf("Error\nThread join failed.\n"));
		i++;
	}
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