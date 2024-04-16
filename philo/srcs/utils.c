#include "../includes/philo.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	sign = 1;
	result = 0;
	while ((nptr[i] > 8 && nptr[i] < 14) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

void print_status(t_data *data, int philo, const char *status) 
{
    long long current_time;
	
	current_time = get_time();
	
	pthread_mutex_lock(&(data->print));
    printf("%lld %d ", current_time - data->start_time, philo + 1);
	printf("%s\n", status);
	pthread_mutex_unlock(&(data->print));
	return ;
}