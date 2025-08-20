/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 09:35:53 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/20 15:11:25 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char **argv)
{
	t_sim	*sim;
	int		err;

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
