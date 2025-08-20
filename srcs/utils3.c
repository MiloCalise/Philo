/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:30:52 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/20 15:55:03 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	hold(t_philo *philo)
{
	if (philo->id % 2 != 0)
		usleep(300);
}

int	check_status(t_philo *philo)
{
	if (philo->sim->nb_to_eat != 0)
	{
		if (philo->meals_eaten >= philo->sim->nb_to_eat)
		{
			pthread_mutex_lock(&philo->sim->dead_lock);
			philo->sim->dead_flag = 1;
			pthread_mutex_unlock(&philo->sim->dead_lock);
			return (1);
		}
	}
	if (get_time_in_ms() - philo->last_meal > philo->sim->time_to_die)
	{
		pthread_mutex_lock(&philo->sim->dead_lock);
		philo->sim->dead_flag = 1;
		pthread_mutex_unlock(&philo->sim->dead_lock);
		return (1);
	}
	return (0);
}

int	lock_order(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		if (normal_lock(philo) == 1)
			return (1);
	}
	else
	{
		if (odd_lock(philo) == 1)
			return (1);
	}
	return (0);
}

void	unlock_order(t_philo *philo)
{
	if (philo->has_left_fork)
	{
		pthread_mutex_unlock(philo->l_fork);
		philo->has_left_fork = 0;
	}
	if (philo->has_right_fork)
	{
		pthread_mutex_unlock(philo->r_fork);
		philo->has_right_fork = 0;
	}
	print_action(philo, "is sleeping");
	smart_sleep(philo->sim->time_to_sleep, philo);
}

void	*monitoring(void *arg)
{
	t_sim	*sim;
	int		id;

	id = 0;
	sim = (t_sim *)arg;
	while (1)
	{
		if (sim->nb_to_eat != 0)
		{
			if (check_meals(sim) == 1)
				return (NULL);
		}
		if (check_death(sim, id) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
