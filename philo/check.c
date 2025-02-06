/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamgar <zamgar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 09:59:37 by zamgar            #+#    #+#             */
/*   Updated: 2025/02/06 19:24:53 by zamgar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_dead(t_philo *philo, size_t time_to_die)
{
	if ((actual_time() - philo->last_time_eat) >= time_to_die)
		return (1);
	return (0);
}

int	meal_counter(t_philo *philo)
{
	int	value;

	pthread_mutex_lock(philo->check_eat);
	value = philo->eaten;
	pthread_mutex_unlock(philo->check_eat);
	return (value);
}

void	mega_check(t_philo *philo)
{
	int	i;

	while (check_finish(&philo[0]) == 0)
	{
		i = -1;
		while (++i < philo[0].philo_nb && !check_death(&philo[0]))
		{
			pthread_mutex_lock(philo[0].check_eat);
			if ((actual_time() - philo[i].last_time_eat)
				>= philo[0].time_to_die && philo[i].is_eating == 0)
			{
				set_death(philo, i);
				pthread_mutex_unlock(philo[0].check_eat);
				return ;
			}
			pthread_mutex_unlock(philo[0].check_eat);
		}
		i = 0;
		while (i < philo[0].philo_nb && philo[0].eat_nb != (size_t) - 1
			&& (size_t)meal_counter(&philo[i]) >= philo[0].eat_nb)
			i++;
		if (i == philo[0].philo_nb)
			set_finish(philo);
		usleep(10);
	}
}
