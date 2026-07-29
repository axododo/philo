#include "philo.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	signe;
	int	nb;

	signe = 1;
	i = 0;
	nb = 0;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			signe *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb *= 10;
		nb += nptr[i] - '0';
		i++;
	}
	return (nb * signe);
}
