/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:08:08 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/19 17:07:01 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	odd_lock(t_philo *philo)
{
	if (check_flag(philo) == 1)
		return (1);
	pthread_mutex_lock(philo->r_fork);
	philo->has_right_fork = 1;
	if (check_flag(philo) == 1)
		return (1);
	print_action(philo, "has taken right fork");
	pthread_mutex_lock(philo->l_fork);
	philo->has_left_fork = 1;
	if (check_flag(philo) == 1)
		return (1);
	print_action(philo, "has taken left fork");
	print_action(philo, "is eating");
	return (0);
}

int	normal_lock(t_philo *philo)
{
	if (check_flag(philo) == 1)
		return (1);
	pthread_mutex_lock(philo->l_fork);
	philo->has_left_fork = 1;
	if (check_flag(philo) == 1)
		return (1);
	print_action(philo, "has taken left fork");
	pthread_mutex_lock(philo->r_fork);
	philo->has_right_fork = 1;
	if (check_flag(philo) == 1)
		return (1);
	print_action(philo, "has taken right fork");
	print_action(philo, "is eating");
	return (0);
}

void	match_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->last_meal = get_time_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->sim->meal_lock);
	smart_sleep(philo->sim->time_to_eat, philo);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	check_eaten(t_philo *philo)
{
	if (philo->sim->nb_to_eat != 0)
	{
		if (philo->meals_eaten >= philo->sim->nb_to_eat)
			return (1);
	}
	return (0);
}
