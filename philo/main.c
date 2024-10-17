#include "philo.h"

int	value == 0;

void	init_values(t_philo *philo)
{
	int	i;

	i = 0;
	while (p_nb > 0)
	{
		philo[i]->philo_id = i;
		philo[i]->eating = 0;
		philo[i]->sleeping = 0;
		philo[i]->thinking = 1;
		philo[i]->rfork = 1;
		philo[i]->lfork = 1;
		i++;
		p_nb--;
	}
}

void	routine(t_philo philo)
{
	pthread_mutex_lock(&philo->mutex_fork);
	value++;
	pthread_mutex_unlock(&philo->mutex_fork);
}

void	create_threads(t_philo *philo, int p_nb)
{
	int	i;

	i = 0;
	while (p_nb > 0)
	{
		if (pthread_create(philo[i]->thread, NULL, &routine, philo[i]) != 0)
			exit(0);
		pthread_mutex_init(&philo[i]->mutex_fork);
		i++;
		p_nb--;
	}
}

int main(int argc, char *argv[])
{
	t_philo *philo;
	int	philo_nb = ft_atoi(argv[1]);
	philo = malloc (sizeof(t_philo) * ft_atoi(argv[1]));
	init_values(philo, philo_nb);
	create_threads(philo, philo_nb);

	return (0);
}