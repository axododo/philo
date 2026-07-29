#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
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

/* philo/parsing.c */
int check_args(char **av);
void init_philo(t_data *da);
int procs_args(char **av, t_data *da);
int init_args(int ac, char **av, t_data *da);
/* philo/philo.c */
int eating(t_philo *ph);
void *phi_loop(void *philo);
int summon_philo(t_philo *ph, t_data *da);
/* philo/res.c */
int ft_atoi(const char *nptr);
/* philo/main.c */
long get_time(struct timeval start);
int mprint(t_philo *ph, int mod);
int big_brother(t_philo *ph);
void clean_all(t_data *da);
int main(int ac, char **av);

#endif
