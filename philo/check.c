/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamgar <zamgar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:59:37 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/02 10:04:00 by zamgar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_dead(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(philo->check_eat);
	if ((actual_time() - philo->last_time_eat) >= time_to_die
		&& philo->is_eating == 0)
		return (pthread_mutex_unlock(philo->check_eat), 1);
	pthread_mutex_unlock(philo->check_eat);
	return (0);
}

int	check_death(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo[0].philo_nb)
	{
		if (philo_dead(&philo[i], philo[i].time_to_die))
		{
			write_status(&philo[i], "died");
			pthread_mutex_lock(philo[0].dead_mutex);
			*philo->dead1 = 1;
			pthread_mutex_unlock(philo[0].dead_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_eat(t_philo *philo)
{
	int	i;
	int	check_eat_nb;

	i = 0;
	check_eat_nb = 0;
	if (philo[0].eat_nb == (size_t)-1)
		return (0);
	while (i < philo[0].philo_nb)
	{
		pthread_mutex_lock(philo[i].check_eat);
		if (philo[i].eaten >= philo[i].eat_nb)
			check_eat_nb++;
		pthread_mutex_unlock(philo[i].check_eat);
		i++;
	}
	if (check_eat_nb == philo[0].philo_nb)
	{
		pthread_mutex_lock(philo[0].dead_mutex);
		*philo->dead1 = 1;
		pthread_mutex_unlock(philo[0].dead_mutex);
		return (1);
	}
	return (0);
}

void	*check(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	while (1)
	{
		if (check_death(philo) == 1 || check_eat(philo) == 1)
			break ;
	}
	return (philo_p);
}
