/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamgar <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:59:00 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/02 09:59:02 by zamgar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(size_t ms, t_philo *philo)
{
	size_t	start;

	start = actual_time();
	while ((actual_time() - start) < ms
		&& !check_death(philo) && !check_finish(philo))
		usleep(ms / 10);
}

void	write_status(t_philo *philo, char *action)
{
	size_t	time;

	pthread_mutex_lock(philo->write);
	time = actual_time() - philo->start_time;
	if (!check_finish(philo) && !check_death(philo))
		printf("%ld %d %s\n", time, philo->id, action);
	pthread_mutex_unlock(philo->write);
}

void	destroy_and_free(t_main *main, pthread_mutex_t *philo_forks)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&main->write);
	pthread_mutex_destroy(&main->check_eat);
	pthread_mutex_destroy(&main->dead_mutex);
	pthread_mutex_destroy(&main->finish_mutex);
	while (i < main->philo[0].philo_nb)
	{
		pthread_mutex_destroy(&philo_forks[i]);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_main			main;
	t_philo			philo[200];
	pthread_mutex_t	philo_forks[200];

	if (argc < 5 || argc > 6)
		return (printf("Wrong numbers of arguments\n"), 1);
	if (ft_atoi(argv[1]) <= 0)
		exit(printf("Wrong arguments\n"));
	if (ft_atoi(argv[2]) <= 0 || ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0)
		exit(printf("Wrong arguments\n"));
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) < 0)
			exit(printf("Wrong arguments\n"));
	}
	init_values(&main, philo, philo_forks, argv);
	if (main.philo[0].philo_nb == 1)
		create_one_thread(&main, philo_forks);
	else
		create_threads(&main, philo_forks);
	destroy_and_free(&main, philo_forks);
	return (0);
}
