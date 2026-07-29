#include "philo.h"
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

long get_time(struct timeval start) {
  struct timeval now;
  gettimeofday(&now, NULL);
  long result =
      (now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000;
  return (result);
}

int mprint(t_philo *ph, int mod) {
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

int big_brother(t_philo *ph) {
  long dif, now;
  int i;

  pthread_mutex_lock(&ph->da->stop);
  i = 0;
  while (i < ph->da->nb_philo) {
    now = get_time(ph->da->start);
    dif = now - ph->da->ph[i].last_meals;

    if (dif > ph->da->time_die && ph->da->ph[i].nb_meals != ph->da->nb_goal) {
      mprint(&ph->da->ph[i], 5);
      ph->rip = 1;
      break;
    }
    if (ph->da->ph[i].nb_meals == ph->da->nb_goal)
      ph->nb_meals--;
    i++;
  }
  if (ph->rip) {
    i = 0;
    while (i < ph->da->nb_philo) {
      ph->da->ph[i].rip = 1;
      i++;
    }
  }
  pthread_mutex_unlock(&ph->da->stop);
  return (1);
}

void clean_all(t_data *da) {
  int i;

  i = 0;
  while (i < da->nb_philo + 1) {
    pthread_mutex_destroy(&da->forks[i++]);
  }
  pthread_mutex_destroy(&da->print);
  free(da->ph);
  free(da->forks);
}

int main(int ac, char **av) {
  t_data da;
  t_philo *ph;

  if (init_args(ac, av, &da))
    return (1);
  init_philo(&da);
  ph = da.ph;
  summon_philo(ph, &da);
  clean_all(&da);
  return (0);
}
