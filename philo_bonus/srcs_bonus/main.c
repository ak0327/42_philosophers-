/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:51:38 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 15:53:57 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo_bonus.h"

/*
 index of philo and forks are following:
  fork   4forks in middle of the table
  philo  0  1  2  3
*/

int	run_pthread(t_philo_info *philo)
{
	int	ret_value;

//	printf("(%zu)run\n", philo->idx + 1);
	ret_value = SUCCESS;
	if (pthread_create(&philo->philo_tid, NULL, (void *)routine, (void *)philo) != SUCCESS)
		return (PROCESS_ERROR);
	if (pthread_create(&philo->monitor_tid, NULL, (void *)monitor, (void *)philo) != SUCCESS)
		return (PROCESS_ERROR);
	if (pthread_join(philo->philo_tid, (void *)&ret_value) != SUCCESS)
		return (PROCESS_ERROR);
	if (pthread_join(philo->monitor_tid, NULL) != SUCCESS)
		return (PROCESS_ERROR);
	printf("(%zu)end\n", philo->idx + 1);
	return (ret_value);
}

void	terminate_child_process(t_info *info)
{
	size_t			idx;

	idx = 0;
	while (info->num_of_philos)
	{
		kill(info->philo_info[idx].pid, SIGINT);
		idx++;
	}
}

int	wait_and_terminate_process(t_info *info)
{
	int				ret_value;
	size_t			idx;

	idx = 0;
	while (idx < info->num_of_philos)
	{
		waitpid(-1, &ret_value, 0);
		ret_value = WEXITSTATUS(ret_value);
//		printf("wait while ret:%d\n", ret_value);
		if (ret_value == PHILO_DIED)
		{
			kill(0, SIGINT);
//			terminate_child_process(info);
			break ;
		}
		if (ret_value == PROCESS_ERROR)
		{
			kill(0, SIGINT);
//			terminate_child_process(info);
			break ;
		}
		idx++;
	}
	printf("wait ret:%d\n", ret_value);
	return (ret_value);
}

int	start_routine(t_info *info)
{
	const time_t	now_time = get_unix_time_ms();
	size_t			idx;

	idx = 0;
	while (idx < info->num_of_philos)
	{
		info->philo_info[idx].start_time = now_time;
		info->philo_info[idx].pid = fork();
		if (info->philo_info[idx].pid < 0)
			return (PROCESS_ERROR);
		if (info->philo_info[idx].pid == 0)
			exit (run_pthread(&info->philo_info[idx]));
		idx++;
	}
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_info	*info;
	int		ret_value;

	info = NULL;
	ret_value = init_info(argc, argv, &info);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, info, EXIT_FAILURE));
//	printf("main-1\n");
	ret_value = start_routine(info);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, info, EXIT_FAILURE));
//	printf("main-2\n");

	ret_value = wait_and_terminate_process(info);
	if (ret_value == PROCESS_ERROR)
		return (print_err_msg_and_free(ret_value, info, EXIT_FAILURE));
//	printf("main-3\n");

	ret_value = destroy_info(info);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, info, EXIT_FAILURE));
//	printf("main-4\n");
	return (EXIT_SUCCESS);
}

#ifdef LEAKS

__attribute__((destructor))
static void	destructor(void)
{
	system("leaks -q philo_bonus");
}

#endif
