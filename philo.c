#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int eating(t_philo *ph) {
  int lwfork;
  int hgfork;

  if (ph->lFork < ph->rFork) {
    lwfork = ph->lFork;
    hgfork = ph->rFork;
  } else {
    lwfork = ph->rFork;
    hgfork = ph->lFork;
  }
  pthread_mutex_lock(&ph->da->forks[lwfork]);
  mprint(ph, 1);
  if (ph->rip) {
    pthread_mutex_unlock(&ph->da->forks[lwfork]);
    return (0);
  }
  pthread_mutex_lock(&ph->da->forks[hgfork]);
  mprint(ph, 1);
  mprint(ph, 2);
  pthread_mutex_lock(&ph->da->print);
  ph->last_meals = get_time(ph->da->start);
  ph->nb_meals++;
  pthread_mutex_unlock(&ph->da->print);
  usleep(ph->da->time_eat);
  pthread_mutex_unlock(&ph->da->forks[hgfork]);
  pthread_mutex_unlock(&ph->da->forks[lwfork]);
  return (0);
}

/*int eating(t_philo *ph) {
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
}*/

int solo_eat(t_philo *ph) {
  pthread_mutex_lock(&ph->da->forks[ph->lFork]);
  mprint(ph, 1);
  mprint(ph, 5);
  ph->rip = 1;
  pthread_mutex_unlock(&ph->da->forks[ph->lFork]);
  return (0);
}

void *phi_loop(void *philo) {
  t_philo *ph;
  ph = philo;
  if (ph->da->nb_philo == 1 && ph->id == 1) {
    solo_eat(ph);
    return (0);
  }
  if (ph->id % 2 == 0)
    usleep(500);
  if (ph->id >= 0 && ph->da->nb_philo > 1) {
    while (1) {
      if (ph->rip == 1)
        break;
      mprint(ph, 3);
      usleep(125);
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
  i = 0;
  while (i < da->nb_philo + 1) {
    pthread_join(ph[i].stone, NULL);
    i++;
  }
  return (0);
}
