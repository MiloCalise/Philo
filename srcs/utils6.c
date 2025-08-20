/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:35:08 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/20 10:47:25 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	clear_forks(t_philo *philo)
{
	if (philo->has_left_fork == 1)
	{
		philo->has_left_fork = 0;
		pthread_mutex_unlock(philo->l_fork);
	}
	if (philo->has_right_fork == 1)
	{
		philo->has_right_fork = 0;
		pthread_mutex_unlock(philo->r_fork);
	}
}

void	meal_init(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->sim->meal_lock);
}

void	*routine_solo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	meal_init(philo);
	print_action(philo, "has taken left fork");
	smart_sleep(philo->sim->time_to_die, philo);
	print_action(philo, "has died");
	return (NULL);
}

int	solo_thread(t_sim *sim)
{
	sim->start_time = get_time_in_ms();
	if (pthread_create(&sim->philo[0].thread, NULL, &routine_solo,
			(void *)&sim->philo[0]) != 0)
		return (printf("Failed to create thread\n"), -1);
	if (pthread_join(sim->philo[0].thread, NULL) != 0)
		return (printf("Failed to join thread\n"), -1);
	return (0);
}
