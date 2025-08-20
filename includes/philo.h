/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:10:25 by miltavar          #+#    #+#             */
/*   Updated: 2025/08/20 15:20:33 by miltavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				meals_eaten;
	int				has_left_fork;
	int				has_right_fork;
	long			last_meal;
	struct s_sim	*sim;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}	t_philo;

typedef struct s_sim
{
	int				dead_flag;
	int				nb_philo;
	int				nb_to_eat;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	meal_lock;
	t_philo			*philo;
}	t_sim;

int		grab_philos(char *s);
int		solo_thread(t_sim *sim);
int		get_sim(int argc, char **argv, t_sim *sim);
int		get_sim2(char **argv, t_sim *sim);
int		check_status(t_philo *philo);
int		threads(t_sim *sim);
int		lock_order(t_philo *philo);
int		check_death(t_sim *sim, int id);
int		check_meals(t_sim *sim);
int		start_monitoring(t_sim *sim, pthread_t *monit);
int		stop_monitoring(pthread_t *monit);
int		check_flag(t_philo *philo);
int		odd_lock(t_philo *philo);
int		normal_lock(t_philo *philo);
int		check_eaten(t_philo *philo);
int		ft_strcmp(const char *s1, const char *s2);

long	grab_time(char *s);
long	get_time_in_ms(void);

void	free_all(t_sim *sim);
void	hold(t_philo *philo);
void	unlock_order(t_philo *philo);
void	print_action(t_philo *philo, char *msg);
void	match_meal(t_philo *philo);
void	smart_sleep(long duration, t_philo *philo);
void	clear_forks(t_philo *philo);
void	meal_init(t_philo *philo);

void	*monitoring(void *arg);

#endif
