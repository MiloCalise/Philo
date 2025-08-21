/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:22:46 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/21 10:43:24 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	grab_philos(char *s)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (s[i])
	{
		if (s[i] > 57 || s[i] < 48)
			return (printf("Not digits\n"), -1);
		res = res * 10 + s[i] - 48;
		i++;
	}
	if (res <= 0 || res > 200)
		return (printf("Too much or too little philos\n"), -1);
	return (res);
}

long	grab_time(char *s)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (s[i])
	{
		if (s[i] > 57 || s[i] < 48)
			return (printf("Not digits\n"), -1);
		res = res * 10 + s[i] - 48;
		i++;
	}
	if (res <= 59)
		return (printf("Time has to be at least 60ms\n"), -1);
	return (res);
}

void	free_all(t_sim *sim)
{
	int	i;

	if (!sim)
		return ;
	if (sim->forks)
	{
		i = 0;
		while (i < sim->nb_philo)
			pthread_mutex_destroy(&sim->forks[i++]);
		free(sim->forks);
	}
	pthread_mutex_destroy(&sim->dead_lock);
	pthread_mutex_destroy(&sim->write_lock);
	pthread_mutex_destroy(&sim->meal_lock);
	if (sim->philo)
		free(sim->philo);
	free(sim);
}

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	get_sim2(char **argv, t_sim *sim)
{
	int	i;

	sim->time_to_eat = grab_time(argv[3]);
	sim->time_to_sleep = grab_time(argv[4]);
	if (sim->time_to_sleep == -1 || sim->time_to_eat == -1)
		return (-1);
	i = 0;
	while (i < sim->nb_philo)
		pthread_mutex_init(&sim->forks[i++], NULL);
	i = 0;
	while (i < sim->nb_philo)
	{
		sim->philo[i].id = i + 1;
		sim->philo[i].has_left_fork = 0;
		sim->philo[i].has_right_fork = 0;
		sim->philo[i].meals_eaten = 0;
		sim->philo[i].last_meal = 0;
		sim->philo[i].sim = sim;
		sim->philo[i].l_fork = &sim->forks[i];
		sim->philo[i].r_fork = &sim->forks[(i + 1) % sim->nb_philo];
		i++;
	}
	return (1);
}
