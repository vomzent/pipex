/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/07/26 12:12:24 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	pid_t pid;

	if (argc < 2)
		return (ft_dprintf(2, "Usage: %s <program>\n", argv[0]), 0);
	pid = fork();
	if (pid == -1)
		return (perror("Error duplicating"), 0);
	if (pid == 0)
	{
		execvp(argv[1], argv + 1);
		return (perror("Error transitioning child"), 0);
	}
	else
	{
		waitpid(pid, 0 , 0);
		ft_printf("I am not the child\nMy child was %d\n", pid);
	}
	return (0);
}
