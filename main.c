#include "philo.h"


int check_args(char **av) {
  int i;
  int y;

  i = 0;
  y = 0;
  while (av[y]){
    while (av[y][i]) {
        if (av[y][i] < '0' || av[y][i] > '9')
          return(1);
      i++;
    }
    y++;
  }
  return(0);
}

void procs_args(char **av, t_data da) {
  da->nb_philo = ft_atoi(av[1]);
  da->time_die = ft_atoi(av[2]);
  da->time_eat = ft_atoi(av[3]);
  da->time_sleep = ft_atoi(av[4]);
  if (av[5])
      da->ds->nb_goal = ft_atoi(av[5]);
  da->ph = malloc(sizeof(da->ph) * da->nb_philo);
  if (!da->ph)
    return(1);
}

int init_args(int ac, char **av, t_data da) {
  
  if (ac < 5 || ac > 6)
    return(1);
  if (check_args(av))
    return(1);
  if (procs_args(av, ph))
    return(1);
}



int int main(int ac, char **av)
{
  t_data da;
  if (init_args(ac, av, &da))
      return(1);
  // main loop
  // clean
}
