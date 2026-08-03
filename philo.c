#include "philo.h"
#include <pthread.h>
#include <unistd.h>

void	choose_fork(t_philo *ph, int *lwfork, int *hgfork)
{
	if (ph->lfork < ph->rfork)
	{
		*lwfork = ph->lfork;
		*hgfork = ph->rfork;
	}
	else
	{
		*lwfork = ph->rfork;
		*hgfork = ph->lfork;
	}
}

int	eating(t_philo *ph)
{
	int	lwfork;
	int	hgfork;

	choose_fork(ph, &lwfork, &hgfork);
	pthread_mutex_lock(&ph->da->forks[lwfork]);
	mprint(ph, 1);
	if (is_dead(ph))
	{
		pthread_mutex_unlock(&ph->da->forks[lwfork]);
		return (0);
	}
	pthread_mutex_lock(&ph->da->forks[hgfork]);
	mprint(ph, 1);
	mprint(ph, 2);
	pthread_mutex_lock(&ph->da->stop);
	ph->last_meals = get_time(ph->da->start);
	ph->nb_meals++;
	pthread_mutex_unlock(&ph->da->stop);
	usleep(ph->da->time_eat);
	pthread_mutex_unlock(&ph->da->forks[hgfork]);
	pthread_mutex_unlock(&ph->da->forks[lwfork]);
	return (0);
}

int	solo_eat(t_philo *ph)
{
	pthread_mutex_lock(&ph->da->forks[ph->lfork]);
	mprint(ph, 1);
	mprint(ph, 5);
	ph->rip = 1;
	pthread_mutex_unlock(&ph->da->forks[ph->lfork]);
	return (0);
}

void	routine(t_philo *ph)
{
	while (1)
	{
		if (is_dead(ph))
			break ;
		mprint(ph, 3);
		usleep(125);
		eating(ph);
		if (ph->nb_meals == ph->da->nb_goal)
			break ;
		if (is_dead(ph))
			break ;
		mprint(ph, 4);
		usleep(ph->da->time_sleep);
		if (is_dead(ph))
			break ;
	}
}

void	*phi_loop(void *philo)
{
	t_philo	*ph;

	ph = philo;
	if (ph->da->nb_philo == 1 && ph->id == 1)
	{
		solo_eat(ph);
		return (0);
	}
	if (ph->id % 2 == 0)
		usleep(500);
	if (ph->id >= 0 && ph->da->nb_philo > 1)
	{
		routine(ph);
	}
	else if (ph->id == -2 && ph->da->nb_philo > 1)
	{
		while (!is_dead(ph) && ph->nb_meals > 0)
		{
			big_brother(ph);
			usleep(500);
		}
	}
	return (0);
}
