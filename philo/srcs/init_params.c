/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 18:45:08 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutex(t_params *params);
static int	init_alloc(t_params **params);

int	init_params(int argc, char **argv, t_params **params)
{
	int	is_process_success;

	if (!(5 <= argc && argc <= 6))
		return (INVALID_ARG_COUNT);
	*params = (t_params *)malloc(sizeof(t_params));
	if (!*params)
		return (PROCESS_ERROR);

	memset(*params, 0, sizeof(t_params));

	is_process_success = true;
	is_process_success |= get_input_args(argv, *params);
	is_process_success |= init_alloc(params);
	is_process_success |= init_mutex(*params);

	(*params)->is_died = false;
	(*params)->died_idx = -1;

	if (is_process_success)
		return (SUCCESS);
	free_params(params);
	return (FAILURE);
}

static int	init_mutex(t_params *params)
{
	size_t	idx;

	if (pthread_mutex_init(&params->print_mutex, NULL) != SUCCESS)
		return (FAILURE);

	params->fork_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * params->num_of_philos);
	if (!params->fork_mutex)
		return (FAILURE);

	if (pthread_mutex_init(&params->died_mutex, NULL) != SUCCESS)
		return (FAILURE);

	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_mutex_init(&params->fork_mutex[idx], NULL) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}

static int	init_alloc(t_params **params)
{
	size_t	idx;

	(*params)->thread_id = (pthread_t *)malloc(sizeof(pthread_t) * (*params)->num_of_philos);
	(*params)->philo_info = (t_philo_info *)malloc(sizeof(t_philo_info) * (*params)->num_of_philos);

	(*params)->state = (int *)malloc(sizeof(int) * (*params)->num_of_philos);
	(*params)->held_by = (ssize_t *)malloc(sizeof(ssize_t) * (*params)->num_of_philos);
	(*params)->prev_used_by = (ssize_t *)malloc(sizeof(ssize_t) * (*params)->num_of_philos);

	if (!(*params)->thread_id || !(*params)->philo_info \
 || !(*params)->state || !(*params)->held_by || !(*params)->prev_used_by)
		return (FAILURE);

	idx = 0;
	while (idx < (*params)->num_of_philos)
	{
		(*params)->philo_info[idx].idx = idx;
		(*params)->philo_info[idx].first_take = min_size(idx, (idx + 1) % (*params)->num_of_philos);
		(*params)->philo_info[idx].second_take = max_size(idx, (idx + 1) % (*params)->num_of_philos);
		(*params)->philo_info[idx].start_time = 0;
		(*params)->philo_info[idx].eat_times = 0;
		(*params)->philo_info[idx].params_ptr = *params;

		(*params)->state[idx] = FORK_DIRTY;
		(*params)->prev_used_by[idx] = -1;
		(*params)->held_by[idx] = -1;
	}
	return (SUCCESS);
}
