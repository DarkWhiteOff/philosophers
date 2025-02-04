/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamgar <zamgar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:59:27 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/02 10:03:26 by zamgar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->lfork);
	write_status(philo, "has taken a fork");
	if (philo->philo_nb == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->lfork);
		return ;
	}
	pthread_mutex_lock(philo->rfork);
	write_status(philo, "has taken a fork");
	philo->is_eating = 1;
	write_status(philo, "is eating");
	pthread_mutex_lock(philo->check_eat);
	philo->last_time_eat = actual_time();
	philo->eaten++;
	pthread_mutex_unlock(philo->check_eat);
	ft_usleep(philo->time_to_eat);
	philo->is_eating = 0;
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
}

int	check_end(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead1 == 1)
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

void	*routine(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (check_end(philo) == 0)
	{
		eat(philo);
		write_status(philo, "is sleeping");
		ft_usleep(philo->time_to_sleep);
		write_status(philo, "is thinking");
	}
	return (philo_p);
}

void	create_threads(t_main *main, pthread_mutex_t *philo_forks)
{
	int	i;

	i = 0;
	if (pthread_create(&main->checker, NULL, &check, main->philo) != 0)
		destroy_and_free(main, philo_forks);
	while (i < main->philo[0].philo_nb)
	{
		if (pthread_create(&main->philo[i].thread, NULL, &routine
				, &main->philo[i]) != 0)
			destroy_and_free(main, philo_forks);
		i++;
	}
	i = 0;
	if (pthread_join(main->checker, NULL) != 0)
		destroy_and_free(main, philo_forks);
	while (i < main->philo[0].philo_nb)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			destroy_and_free(main, philo_forks);
		i++;
	}
	return ;
}
