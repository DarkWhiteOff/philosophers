/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:59:27 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/06 22:38:48 by marvin           ###   ########.fr       */
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
	ft_usleep(philo->time_to_die, philo);
	write_status(philo, "died");
	return (philo_p);
}

void	eat_action(t_philo *philo)
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
	pthread_mutex_lock(philo->check_eat);
	philo->last_time_eat = actual_time();
	philo->eaten++;
	pthread_mutex_unlock(philo->check_eat);
	write_status(philo, "is eating");
	philo->is_eating = 0;
	ft_usleep(philo->time_to_eat, philo);
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
}

void	*routine(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	if (philo->id % 2 == 0)
		ft_usleep(15, philo);
	while (check_death(philo) == 0)
	{
		eat_action(philo);
		if (check_finish(philo) == 1)
			break ;
		write_status(philo, "is sleeping");
		ft_usleep(philo->time_to_sleep, philo);
		write_status(philo, "is thinking");
		usleep(100);
	}
	return (philo_p);
}

void	create_one_thread(t_main *main, pthread_mutex_t *philo_forks)
{
	int	i;

	i = 0;
	main->philo[i].start_time = actual_time();
	main->philo[i].last_time_eat = actual_time();
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
		main->philo[i].start_time = actual_time();
		main->philo[i].last_time_eat = actual_time();
		if (pthread_create(&main->philo[i].thread, NULL, &routine
				, &main->philo[i]) != 0)
			destroy_and_free(main, philo_forks);
		i++;
	}
	mega_check(main->philo);
	i = 0;
	while (i < main->philo[0].philo_nb)
	{
		if (pthread_join(main->philo[i].thread, NULL) != 0)
			destroy_and_free(main, philo_forks);
		i++;
	}
	return ;
}
