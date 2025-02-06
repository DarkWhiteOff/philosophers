/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:59:11 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/06 22:50:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_main(t_main *main, t_philo *philo)
{
	pthread_mutex_init(&main->write, NULL);
	pthread_mutex_init(&main->check_eat, NULL);
	pthread_mutex_init(&main->dead_mutex, NULL);
	pthread_mutex_init(&main->finish_mutex, NULL);
	main->dead = 0;
	main->finish = 0;
	main->philo = philo;
	return ;
}

void	init_forks(t_philo *philo, pthread_mutex_t *philo_forks, char *philonb)
{
	int	i;
	int	j;
	int	p_nb;

	i = 0;
	while (i < ft_atoi(philonb))
	{
		pthread_mutex_init(&philo_forks[i], NULL);
		i++;
	}
	i = 0;
	j = 0;
	p_nb = (ft_atoi(philonb) - 1);
	philo[i].lfork = &philo_forks[i];
	if (ft_atoi(philonb) > 1)
		philo[i].rfork = &philo_forks[p_nb];
	i += 1;
	while (i < p_nb + 1)
	{
		philo[i].lfork = &philo_forks[i];
		philo[i].rfork = &philo_forks[j];
		i++;
		j++;
	}
	return ;
}

void	init_philo2(t_main *main, t_philo *philo, int i)
{
	philo[i].last_time_eat = 0;
	philo[i].start_time = 0;
	philo[i].write = &main->write;
	philo[i].check_eat = &main->check_eat;
	philo[i].dead_mutex = &main->dead_mutex;
	philo[i].finish_mutex = &main->finish_mutex;
	philo[i].dead1 = &main->dead;
	philo[i].finish1 = &main->finish;
}

void	init_philo(t_main *main, t_philo *philo, char *argv[])
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philo[i].id = i + 1;
		philo[i].eaten = 0;
		philo[i].is_eating = 0;
		philo[i].philo_nb = ft_atoi(argv[1]);
		philo[i].time_to_die = ft_atoi(argv[2]);
		philo[i].time_to_eat = ft_atoi(argv[3]);
		philo[i].time_to_sleep = ft_atoi(argv[4]);
		if (argv[5])
			philo[i].eat_nb = ft_atoi(argv[5]);
		else
			philo[i].eat_nb = -1;
		init_philo2(main, philo, i);
		i++;
	}
	return ;
}

void	init_values(t_main *m, t_philo *ph, pthread_mutex_t *forks, char *av[])
{
	init_main(m, ph);
	init_forks(ph, forks, av[1]);
	init_philo(m, ph, av);
	return ;
}
