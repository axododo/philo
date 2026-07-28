#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_data {
  struct timeval start;
  int nb_philo;
  int time_die;
  int time_eat;
  int time_sleep;
  int nb_goal;
  pthread_mutex_t *forks;
  pthread_mutex_t print;
  struct s_philo *ph;
} t_data;

typedef struct s_philo {
  int id;
  int nb_meals;
  long last_meals;
  int rip;
  int rFork;
  int lFork;
  pthread_t stone;
  struct s_data *da;
} t_philo;

int ft_atoi(const char *nptr);

#endif
