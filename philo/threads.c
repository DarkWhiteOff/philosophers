/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:59:27 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/06 04:24:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine_one(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	pthread_mutex_lock(philo->lfork);
	write_status(philo, "has taken a fork");
	pthread_mutex_unlock(philo->lfork);
	ft_usleep(philo->time_to_die);
	write_status(philo, "died");		
	return (philo_p);
}

void	eat(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->lfork);
		write_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->rfork);
		write_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->rfork);
		write_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->lfork);
		write_status(philo, "has taken a fork");
	}
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
		ft_usleep(15);
	while (check_end(philo) == 0)
	{
		eat(philo);
		write_status(philo, "is sleeping");
		ft_usleep(philo->time_to_sleep);
		write_status(philo, "is thinking");
		usleep(100);
	}
	return (philo_p);
}

void	create_one_thread(t_main *main, pthread_mutex_t *philo_forks)
{
	int	i;

	i = 0;
	if (pthread_create(&main->philo[i].thread, NULL, &routine_one
			, &main->philo[i]) != 0)
		destroy_and_free(main, philo_forks);
	if (pthread_join(main->philo[i].thread, NULL) != 0)
		destroy_and_free(main, philo_forks);
	return ;
}

void	create_threads(t_main *main, pthread_mutex_t *philo_forks)
{
	int	i;

	i = 0;
	while (i < main->philo[0].philo_nb)
	{
		if (pthread_create(&main->philo[i].thread, NULL, &routine
				, &main->philo[i]) != 0)
			destroy_and_free(main, philo_forks);
		i++;
	}
	while (1)
	{
		if (check_death(main->philo) == 1 || check_eat(main->philo) == 1)
			break ;
		usleep(10);
	}
	i = 0;
	while (i < main->philo[0].philo_nb)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			destroy_and_free(main, philo_forks);
		i++;
	}
	return ;
}
