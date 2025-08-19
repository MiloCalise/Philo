/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:57:19 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/19 15:58:37 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_death(t_sim *sim)
{
	int	id;

	id = 0;
	while (id < sim->nb_philo)
	{
		pthread_mutex_lock(&sim->meal_lock);
		if (get_time_in_ms() - sim->philo[id].last_meal > sim->time_to_die)
		{
			pthread_mutex_unlock(&sim->meal_lock);
			print_action(&sim->philo[id], "has died");
			return (1);
		}
		pthread_mutex_unlock(&sim->meal_lock);
		id++;
	}
	return (0);
}

int	check_meals(t_sim *sim)
{
	int	id;
	int	check;

	id = 0;
	check = 0;
	while (id < sim->nb_philo)
	{
		pthread_mutex_lock(&sim->meal_lock);
		if (sim->philo[id].meals_eaten >= sim->nb_to_eat)
		{
			check++;
			pthread_mutex_unlock(&sim->meal_lock);
		}
		else
			pthread_mutex_unlock(&sim->meal_lock);
		id++;
	}
	if (check == sim->nb_philo)
	{
		pthread_mutex_lock(&sim->dead_lock);
		sim->dead_flag = 1;
		pthread_mutex_unlock(&sim->dead_lock);
		return (1);
	}
	return (0);
}

int	start_monitoring(t_sim *sim, pthread_t *monit)
{
	int				err;

	err = pthread_create(monit, NULL, &monitoring, (void *)sim);
	if (err != 0)
		return (printf("Failed to create monitoring thread\n"), -1);
	return (1);
}

int	stop_monitoring(pthread_t *monit)
{
	int	err;

	err = pthread_join(*monit, NULL);
	if (err != 0)
		return (printf("Failed to join monitoring thread\n"), -1);
	return (1);
}

int	check_flag(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->dead_lock);
	if (philo->sim->dead_flag == 1)
	{
		pthread_mutex_unlock(&philo->sim->dead_lock);
		clear_forks(philo);
		return (1);
	}
	pthread_mutex_unlock(&philo->sim->dead_lock);
	if (check_eaten(philo) == 1)
	{
		clear_forks(philo);
		return (1);
	}
	return (0);
}
