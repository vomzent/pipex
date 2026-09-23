/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                             :+:    :+:           */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/23 15:22:49 by vcoevert     ########   odam.nl          */
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

	if (!access(name, X_OK))
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
		if (!access(ret, X_OK))
			return (free_dbptr(envp), free(name), ret);
		free(ret);
		path++;
	}
	return (free_dbptr(envp), free(name), (char *)0);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t 	pid1;
	pid_t	pid2;
	char	**p_path;
	char	**p1_argv;
	char	**p2_argv;
	int		pipe_fd[2];

	if (argc < 3)
		return (ft_dprintf(2, "Usage: %s <program> <program>\n", argv[0]), 0);
	if (pipe(pipe_fd) == -1)
		return (perror("Pipe error"), 0);
	p1_argv = ft_split(argv[1], ' ');
	if (!p1_argv)
		return (ft_dprintf(2, "Malloc fail :("));
	p2_argv = ft_split(argv[2], ' ');
	if (!p2_argv)
		return (free_dbptr(p1_argv), ft_dprintf(2, "Malloc fail :("));
	p_path = calloc(3, sizeof(void *));
	if (!p_path)
		return (free_dbptr(p1_argv), free_dbptr(p2_argv), ft_dprintf(2, "Malloc fail :("));
	p_path[0] = find_program_path(p1_argv[0], envp);
	p_path[1] = find_program_path(p2_argv[0], envp);
	if (!p_path[0] || !p_path[1])
		return (free_dbptr(p1_argv), free_dbptr(p2_argv),ft_dprintf(2, "Program not found on path"));
	pid1 = fork();
	if (pid1 == -1)
		return (perror("Error duplicating"), 0);
	if (pid1 == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		execve(p_path[0], p1_argv, envp);
		return (perror("Error transitioning child"), 0);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		execve(p_path[1], p2_argv, envp);
		return (perror("Error transitioning child"), 0);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, 0 ,0);
	waitpid(pid2, 0, 0);
	free_dbptr(p1_argv);
	free_dbptr(p2_argv);
	free_dbptr(p_path);
	ft_printf("I am not the child\nMy child was %d and %d\n", pid1, pid2);
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
