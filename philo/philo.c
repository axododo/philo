#include "philo.h"

int eating(t_philo *ph) {
  if (ph->rip == 1)
    return (0);
  if (ph->id == ph->da->nb_philo && ph->da->nb_philo != 1) {
    pthread_mutex_lock(&ph->da->forks[ph->rFork]);
    mprint(ph, 1);
    pthread_mutex_lock(&ph->da->forks[ph->lFork]);
    mprint(ph, 1);
  } else {

    pthread_mutex_lock(&ph->da->forks[ph->lFork]);
    mprint(ph, 1);
    if (ph->rip == 1)
      return (0);
    if (ph->da->nb_philo > 1)
      pthread_mutex_lock(&ph->da->forks[ph->rFork]);
    else {
      ph->rip = 1;
      mprint(ph, 5);
    } // printf("ici"); // usleep(ph->da->time_die * 500);
    mprint(ph, 1);
  }
  mprint(ph, 2);
  usleep(ph->da->time_eat);
  pthread_mutex_lock(&ph->da->print);
  ph->last_meals = get_time(ph->da->start);
  ph->nb_meals++;
  pthread_mutex_unlock(&ph->da->print);
  pthread_mutex_unlock(&ph->da->forks[ph->rFork]);
  pthread_mutex_unlock(&ph->da->forks[ph->lFork]);
  return (0);
}

void *phi_loop(void *philo) {
  t_philo *ph;
  ph = philo;
  if (ph->id % 2 == 1)
    usleep(1000);
  if (ph->id > 0) {
    while (1) {
      if (ph->rip == 1)
        break;
      mprint(ph, 3);
      // usleep(500 * 1000);
      eating(ph);
      if (ph->nb_meals == ph->da->nb_goal)
        break;
      if (ph->rip == 1)
        break;
      mprint(ph, 4);
      usleep(ph->da->time_sleep);
      if (ph->rip == 1)
        break;
    }
  } else if (ph->id == -2 && ph->da->nb_philo > 1) {
    while (ph->rip == 0 && ph->nb_meals > 0) {
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
  printf("\nexit\n");
  i = 0;
  while (i < da->nb_philo + 1) {
    pthread_join(ph[i].stone, NULL);
    i++;
  }
  return (0);
}
