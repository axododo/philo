#include "philo.h"
#include <pthread.h>

long	get_time(struct timeval start)
{
	struct timeval	now;
	long			result;

	gettimeofday(&now, NULL);
	result =
		(now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec)
			/ 1000;
	return (result);
}

int	is_dead(t_philo *ph)
{
	int	rip;

	rip = 0;
	pthread_mutex_lock(&ph->da->stop);
	if (ph->rip)
	{
		rip = 1;
	}
	pthread_mutex_unlock(&ph->da->stop);
	return (rip);
}

void	clean_all(t_data *da)
{
	int	i;

	i = 0;
	while (i < da->nb_philo + 1)
	{
		pthread_mutex_destroy(&da->forks[i++]);
	}
	pthread_mutex_destroy(&da->print);
	pthread_mutex_destroy(&da->stop);
	free(da->ph);
	free(da->forks);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	signe;
	int	nb;

	signe = 1;
	i = 0;
	nb = 0;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			signe *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb *= 10;
		nb += nptr[i] - '0';
		i++;
	}
	return (nb * signe);
}
