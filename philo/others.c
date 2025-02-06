/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   others.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamgar <zamgar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:59:00 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/06 19:25:15 by zamgar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead1 == 1)
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

int	check_finish(t_philo *philo)
{
	pthread_mutex_lock(philo->finish_mutex);
	if (*philo->finish1 == 1)
		return (pthread_mutex_unlock(philo->finish_mutex), 1);
	pthread_mutex_unlock(philo->finish_mutex);
	return (0);
}

void	set_death(t_philo *philo, int i)
{
	write_status(&philo[i], "died");
	pthread_mutex_lock(philo[0].dead_mutex);
	*philo->dead1 = 1;
	pthread_mutex_unlock(philo[0].dead_mutex);
}

void	set_finish(t_philo *philo)
{
	pthread_mutex_lock(philo[0].finish_mutex);
	*philo->finish1 = 1;
	pthread_mutex_unlock(philo[0].finish_mutex);
}
