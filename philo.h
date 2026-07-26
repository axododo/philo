#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_data {
  int nb_philo;
  int time_die;
  int time_eat;
  int time_sleep;
  int nb_goal;
  struct s_philo *ph;
} t_data;

typedef struct s_philo {
  int id;
  int nb_meals;
  int last_meals; // time last
  int rip;
  pthread_t stone;
  struct s_data *da;
} t_philo;

int ft_atoi(const char *nptr);

#endif
