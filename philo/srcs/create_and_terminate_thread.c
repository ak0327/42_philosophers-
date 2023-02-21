/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 13:52:52 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_params *params)
{
	size_t			idx;
	const time_t	start_time = get_unix_time_ms();

	if (!params)
		return (FAILURE);
	idx = 0;
	while (idx < params->num_of_philos)
	{
		(&params->philo_info[idx])->start_time = start_time;
		if (pthread_create(&params->tid[idx], NULL, do_routine, (void *)&params->philo_id[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
/*
	idx = 1;
	while (idx < params->num_of_philos)
	{
		(&params->philo_info[idx])->start_time = start_time;
		if (pthread_create(&params->tid[idx], NULL, do_routine, (void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
//	usleep(1000);
	idx = 0;
	while (idx < params->num_of_philos)
	{
		(&params->philo_info[idx])->start_time = start_time;
		if (pthread_create(&params->tid[idx], NULL, do_routine, (void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
*/
	return (SUCCESS);
}

int	terminate_threads(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_join(params->tid[idx], NULL) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	return (SUCCESS);
}
