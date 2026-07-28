#include "philo.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

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

long get_time(struct timeval start) {
  struct timeval now;
  gettimeofday(&now, NULL);
  long result =
      (now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000;
  return (result);
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
  da->ph = calloc(da->nb_philo + 2, sizeof(t_philo));
  da->forks = calloc(da->nb_philo + 2, sizeof(pthread_mutex_t));
  da->print = calloc(1, sizeof(pthread_mutex_t));
  int i = 0;
  while (i < da->nb_philo + 1) {
    pthread_mutex_init(&da->forks[i], NULL);
    i++;
  }
  pthread_mutex_init(da->print, NULL);
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

void init_philo(t_data *da) {
  int i;

  i = 0;

  while (i < da->nb_philo) {
    da->ph[i].nb_meals = 0;
    da->ph[i].id = i + 1;
    da->ph[i].rip = 0;
    da->ph[i].last_meals = get_time(da->start);
    da->ph[i].rFork = i;
    da->ph[i].lFork = (i + 1) % da->nb_philo;
    da->ph[i].da = da;
    i++;
  }
  da->ph[i].id = -2;
  da->ph[i].da = da;
  return;
}

int mprint(t_philo *ph, int mod) {
  if (ph->rip == 1)
    return (0);
  pthread_mutex_lock(ph->da->print);
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

  pthread_mutex_unlock(ph->da->print);
  return (0);
}

int big_brother(t_philo *ph) {
  long dif, now;
  int i;

  i = 0;
  while (i < ph->da->nb_philo) {
    now = get_time(ph->da->start);
    dif = now - ph->da->ph[i].last_meals;

    if (dif > ph->da->time_die) {
      mprint(&ph->da->ph[i], 5);
      ph->rip = 1;
      break;
    }
    i++;
  }
  if (ph->rip) {
    i = 0;
    while (i < ph->da->nb_philo) {
      ph->da->ph[i].rip = 1;
      i++;
    }
  }
  return (1);
}

int eating(t_philo *ph) {
  if (ph->rip == 1)
    return (0);

  pthread_mutex_lock(&ph->da->forks[ph->lFork]);
  mprint(ph, 1);
  if (ph->rip == 1)
    return (0);

  pthread_mutex_lock(&ph->da->forks[ph->rFork]);
  mprint(ph, 1);
  mprint(ph, 2);
  usleep(ph->da->time_eat);
  ph->last_meals = get_time(ph->da->start);
  pthread_mutex_unlock(&ph->da->forks[ph->rFork]);
  pthread_mutex_unlock(&ph->da->forks[ph->lFork]);
  return (0);
}

void *phi_loop(void *philo) {
  t_philo *ph;
  ph = philo;
  if (ph->id % 2 == 0)
    usleep(1000);
  if (ph->id > 0) {
    while (1) {
      if (ph->rip == 1)
        break;
      mprint(ph, 3);
      // usleep(500 * 1000);
      eating(ph);
      if (ph->rip == 1)
        break;
      mprint(ph, 4);
      usleep(ph->da->time_sleep);
      if (ph->rip == 1)
        break;
    }
  } else if (ph->id == -2) {
    while (ph->rip == 0) {
      big_brother(ph);
      usleep(500);
    }
  }
  return (0);
}

int summon_philo(t_philo *ph, t_data *da) {
  int i;

  i = 0;
  while (i < da->nb_philo + 1) {
    pthread_create(&ph[i].stone, NULL, &phi_loop, &ph[i]);
    i++;
  }
  i = 0;
  while (i < da->nb_philo + 1) {
    pthread_join(ph[i].stone, NULL);
    i++;
  }
  return (0);
}

int main(int ac, char **av) {
  t_data da;
  t_philo *ph;

  if (init_args(ac, av, &da))
    return (1);
  init_philo(&da);
  ph = da.ph;
  summon_philo(ph, &da);
  // clean
  return (0);
}
