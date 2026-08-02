#include "philo.h"
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

int	mprint(t_philo *ph, int mod)
{
	if (ph->rip == 1 && mod != 5)
		return (0);
	pthread_mutex_lock(&ph->da->print);
	if (mod == 1)
		printf("%li %i has taken a fork\n", get_time(ph->da->start), ph->id);
	else if (mod == 2)
		printf("%li %i is eating\n", get_time(ph->da->start), ph->id);
	else if (mod == 3)
		printf("%li %i is thinking\n", get_time(ph->da->start), ph->id);
	else if (mod == 4)
		printf("%li %i is sleeping\n", get_time(ph->da->start), ph->id);
	else if (mod == 5)
		printf("%li %i died\n", get_time(ph->da->start), ph->id);
	pthread_mutex_unlock(&ph->da->print);
	return (0);
}

void	bb_watching(t_philo *ph)
{
	int		i;
	long	now;
	long	dif;

	i = 0;
	while (i < ph->da->nb_philo)
	{
		now = get_time(ph->da->start);
		dif = now - ph->da->ph[i].last_meals;
		if (dif > ph->da->time_die && ph->da->ph[i].nb_meals != ph->da->nb_goal)
		{
			mprint(&ph->da->ph[i], 5);
			ph->rip = 1;
			break ;
		}
		if (ph->da->ph[i].nb_meals == ph->da->nb_goal)
			ph->nb_meals--;
		i++;
	}
}

int	big_brother(t_philo *ph)
{
	int		i;
	long	dif;
	long	now;

	pthread_mutex_lock(&ph->da->stop);
	i = 0;
	bb_watching(ph);
	if (ph->rip)
	{
		i = 0;
		while (i < ph->da->nb_philo)
		{
			ph->da->ph[i].rip = 1;
			i++;
		}
	}
	pthread_mutex_unlock(&ph->da->stop);
	return (1);
}

int	summon_philo(t_philo *ph, t_data *da)
{
	int	i;

	i = 0;
	while (i < da->nb_philo + 1)
	{
		pthread_create(&ph[i].stone, NULL, &phi_loop, &ph[i]);
		i++;
	}
	i = 0;
	while (i < da->nb_philo + 1)
	{
		pthread_join(ph[i].stone, NULL);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	da;
	t_philo	*ph;

	if (init_args(ac, av, &da))
		return (1);
	init_philo(&da);
	ph = da.ph;
	summon_philo(ph, &da);
	clean_all(&da);
	return (0);
}
