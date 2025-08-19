/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:42:56 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/19 18:00:53 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	get_sim(int argc, char **argv, t_sim *sim)
{
	sim->nb_philo = grab_philos(argv[1]);
	sim->time_to_die = grab_time(argv[2]);
	if (argc == 6)
		sim->nb_to_eat = grab_time(argv[5]);
	else
		sim->nb_to_eat = 0;
	sim->dead_flag = 0;
	if (sim->nb_philo == -1 || sim->time_to_die == -1 || sim->nb_to_eat == -1)
		return (printf("Parsing failed\n"), 0);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->nb_philo);
	if (!sim->forks)
		return (printf("Malloc failed\n"), 0);
	sim->philo = malloc(sizeof(t_philo) * sim->nb_philo);
	if (!sim->philo)
		return (printf("Malloc failed\n"), free(sim->forks), 0);
	pthread_mutex_init(&sim->dead_lock, NULL);
	pthread_mutex_init(&sim->write_lock, NULL);
	pthread_mutex_init(&sim->meal_lock, NULL);
	if (get_sim2(argv, sim) == -1)
		return (-1);
	return (1);
}


void	smart_sleep(long duration, t_philo *philo)
{
	long	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < duration)
	{
		pthread_mutex_lock(&philo->sim->meal_lock);
		if (get_time_in_ms() - philo->last_meal > philo->sim->time_to_die)
		{
			pthread_mutex_unlock(&philo->sim->meal_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->sim->meal_lock);
		usleep(200);
	}
}

void	print_action(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->sim->dead_lock);
	if (philo->sim->dead_flag)
	{
		pthread_mutex_unlock(&philo->sim->dead_lock);
		return ;
	}
	if (ft_strcmp(msg, "has died") == 0)
		philo->sim->dead_flag = 1;
	pthread_mutex_lock(&philo->sim->write_lock);
	printf("%ld %d %s\n", (get_time_in_ms() - philo->sim->start_time),
		philo->id, msg);
	pthread_mutex_unlock(&philo->sim->write_lock);
	pthread_mutex_unlock(&philo->sim->dead_lock);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	meal_init(philo);
	hold(philo);
	while (1)
	{
		if (check_flag(philo) == 1)
			break ;
		if (lock_order(philo) == 1)
			break ;
		match_meal(philo);
		if (check_flag(philo) == 1)
			break ;
		if (check_flag(philo) == 1)
			break ;
		unlock_order(philo);
		if (check_flag(philo) == 1)
			break ;
		match_think(philo);
		if (check_flag(philo) == 1)
			break ;
	}
	clear_forks(philo);
	return (NULL);
}

int	threads(t_sim *sim)
{
	int	i;
	pthread_t	monit;

	i = 0;
	sim->start_time = get_time_in_ms();
	while (i < sim->nb_philo)
	{
		sim->philo[i].last_meal = get_time_in_ms();
		if (pthread_create(&sim->philo[i].thread, NULL, &routine,
				(void *)&sim->philo[i]) != 0)
				return (printf("Failed to create thread\n"), -1);
		i++;
	}
	if (start_monitoring(sim, &monit) == -1)
		return (-1);
	i = 0;
	if (stop_monitoring(&monit) == -1)
		return (-1);
	while (i < sim->nb_philo)
	{
		if (pthread_join(sim->philo[i].thread, NULL) != 0)
			return (printf("Failed to join thread\n"), -1);
		i++;
	}
	return (1);
}
