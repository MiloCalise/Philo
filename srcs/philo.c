/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 09:35:53 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/19 18:01:31 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_death2(t_sim *sim)
{
	int	id;

	id = 0;
	while (id < sim->nb_philo)
	{
		pthread_mutex_lock(&sim->meal_lock);
		if (get_time_in_ms() - sim->philo[id].last_meal > sim->time_to_die)
		{
			pthread_mutex_unlock(&sim->meal_lock);
			return (1);
		}
		pthread_mutex_unlock(&sim->meal_lock);
		id++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_sim	*sim;
	int err;

	if (argc != 5 && argc != 6)
		return (printf("Missing arguments\n"), 1);
	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (printf("Malloc struct failed\n"), 1);
	err = get_sim(argc, argv, sim);
	if (err != 1)
	{
		if (err == 0)
			return (free(sim), 1);
		else
			return (free_all(sim), 1);
	}
	if (sim->nb_philo == 1)
	{
		if (solo_thread(sim) == -1)
			return (free_all(sim), -1);
	}
	if (threads(sim) == -1)
		return (free_all(sim), 1);
	return (free_all(sim), 0);
}

