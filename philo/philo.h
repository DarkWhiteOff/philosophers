#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct s_philosophers
{
	int	eating;
	int	sleeping;
	int	thinking;
	int	rfork;
	int	lfork;
    pthread_t thread;
    pthread_mutex_t mutex_fork;
}	t_philosophers;

size_t	ft_strlen(const char *s);
int	ft_isspace(int c);
int	ft_atoi(const char *str);