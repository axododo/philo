#include "philo.h"
#include <pthread.h>
#include <stdlib.h>

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

int procs_args(char **av, t_data *da) {
  da->nb_philo = ft_atoi(av[1]);
  da->time_die = ft_atoi(av[2]);
  da->time_eat = ft_atoi(av[3]);
  da->time_sleep = ft_atoi(av[4]);
  if (av[5])
    da->nb_goal = ft_atoi(av[5]);
  da->ph = calloc(da->nb_philo + 1,sizeof(t_philo));
  da->ph->stone = calloc(da->nb_philo + 1, sizeof(pthread_t));
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
    da->ph[i].id = i;
    da->ph[i].rip = 0;
    da->ph[i].last_meals = 0;
    i++;
  }
  return;
}

void *phi_loop(void *philo) {
  t_philo *ph;
  ph = philo;
  printf("je dort %i\n", ph->id);
  usleep(500);
  printf("je pense %i\n", ph->id);
  usleep(500);

  return (0);
}

int summon_philo(t_philo *ph, t_data *da) {
  int i;

  i = 0;
  
  while (i < da->nb_philo) {
    pthread_create(&ph[i].stone, NULL, &phi_loop, &ph[i]);
    i++;
  }

  i = 0;
  while(i < da->nb_philo) {
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
  return(0);
}
