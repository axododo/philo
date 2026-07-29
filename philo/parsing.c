#include "philo.h"

int check_args(char **av) {
  int i;
  int y;

  i = 0;
  y = 1;
  while (av[y]) {
    while (av[y][i]) {
      if (av[y][i] < '0' || av[y][i] > '9')
        return (1);
      i++;
    }
    i = 0;
    y++;
  }
  return (0);
}

void init_philo(t_data *da) {
  int i;

  i = 0;

  while (i < da->nb_philo) {
    da->ph[i].nb_meals = 0;
    da->ph[i].id = i + 1;
    da->ph[i].rip = 0;
    da->ph[i].last_meals = get_time(da->start);
    da->ph[i].lFork = i;
    da->ph[i].rFork = (i + 1) % da->nb_philo;
    da->ph[i].da = da;
    i++;
  }
  da->ph[i].id = -2;
  da->ph[i].da = da;
  da->ph[i].nb_meals = da->nb_philo;
  return;
}

int procs_args(char **av, t_data *da) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  da->start = tv;
  da->nb_philo = ft_atoi(av[1]);
  da->time_die = ft_atoi(av[2]);
  da->time_eat = ft_atoi(av[3]) * 1000;
  da->time_sleep = ft_atoi(av[4]) * 1000;
  if (av[5])
    da->nb_goal = ft_atoi(av[5]);
  else
    da->nb_goal = -1;
  da->ph = calloc(da->nb_philo + 2, sizeof(t_philo));
  da->forks = calloc(da->nb_philo + 2, sizeof(pthread_mutex_t));
  // da->print = calloc(1, sizeof(pthread_mutex_t));
  int i = 0;
  while (i < da->nb_philo + 1) {
    pthread_mutex_init(&da->forks[i], NULL);
    i++;
  }
  pthread_mutex_init(&da->print, NULL);
  if (!da->ph)
    return (1);
  return (0);
}

int init_args(int ac, char **av, t_data *da) {

  if (ac < 5 || ac > 6)
    return (1);
  if (check_args(av))
    return (1);
  if (procs_args(av, da))
    return (1);
  return (0);
}
