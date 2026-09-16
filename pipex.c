/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                             :+:    :+:           */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/16 19:15:57 by vcoevert       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void free_dbptr(char **content);

char	*find_program_path(char *name, char **envp)
{
	char	*ret;
	char	**path;

	if (name[0] == '~' || name[0] == '.' || name[0] == '/')
		return (ft_strdup(name));
	name = ft_strjoin("/", name);
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5))
			break;
		envp++;
	}
	envp = ft_split(*envp + 5, ':');
	path = envp;
	while(*path)
	{
		ret = ft_strjoin(*path, name);
		ft_printf("%s\n", ret);
		if (!access(ret, F_OK))
			return (free_dbptr(envp), free(name), ret);
		free(ret);
		path++;
	}
	return (free_dbptr(envp), free(name), (char *)0);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t 	pid;
	char	*p1_path;
	char	**p1_argv;

	if (argc < 2)
		return (ft_dprintf(2, "Usage: %s <program>\n", argv[0]), 0);
	// all allocated shit should be done before here or after execve
	p1_argv = ft_split(argv[1], ' ');
	p1_path = find_program_path(p1_argv[0], envp);
	if (!p1_path)
		return (free_dbptr(p1_argv),ft_dprintf(2, "Program not found on path"));
	pid = fork();
	if (pid == -1)
		return (perror("Error duplicating"), 0);
	if (pid == 0 && p1_path)
	{	
		execve(p1_path, p1_argv, envp);
		return (perror("Error transitioning child"), 0);
	}
	waitpid(pid, 0 , 0);
	free_dbptr(p1_argv);
	free(p1_path);
	ft_printf("I am not the child\nMy child was %d\n", pid);
	return (0);
}

void free_dbptr(char **content) {
	char **head;

	head = content;
	while(*head)
	{
		free(*head);
		head++;
	}
	free(content);
}
