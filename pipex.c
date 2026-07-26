/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/07/26 10:50:53 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// int main(int argc, char **argv, char **envp)
// {
// 	char **i;
//
// 	i = envp;
// 	while (*i)
// 		ft_printf("%s\n", *i++);
// 	return(0);
// }

int	main(int argc, char **argv)
{
	pid_t pid;

	if (argc != 2)
		return (ft_dprintf(2, "Usage: %s <program>\n", argv[0]), 0);
	pid = fork();
	if (pid == -1)
		return (perror("Error duplicating"), 0);
	if (pid == 0)
		ft_printf("I am the child\n");
	else
		ft_printf("I am not the child\n");
	return (0);
}
