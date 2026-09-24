/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                             :+:    :+:           */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/24 19:27:33 by vcoevert     ########   odam.nl          */
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
void free_trptr(char ***content);
int	ptrlen(char *content);

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
		if (!access(ret, X_OK))
			return (free_dbptr(envp), free(name), ret);
		free(ret);
		path++;
	}
	return (free_dbptr(envp), free(name), (char *)0);
}

char	***generate_children_argv(char **argv)
{
	char ***ret;
	char ***p_argv;

	ret = malloc((ptrlen((char *)argv) + 1) * sizeof(void *));
	p_argv = ret;
	if (!p_argv)
		return (ret);
	while (*argv)
	{
		*p_argv = ft_split(*argv, ' ');
		if (!*p_argv)
		{
			free_trptr(ret);
			return (0);
		}
		argv++;
		p_argv++;
	}
	*p_argv = 0;
	return (ret);
}

char **generate_children_path(char ***p_argv, char **envp)
{
	char	**ret;
	char	**p_path;

	ret = malloc((ptrlen((char *)p_argv) + 1) * sizeof(void *));
	p_path = ret;
	if (!p_path)
		return (ret);
	while (*p_argv)
	{
		*p_path = find_program_path(**p_argv, envp);
		if (!*p_path)
		{
			free_dbptr(ret);
			return (0);
		};
		p_argv++;
		p_path++;
	}
	*p_path = 0;
	return (ret);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t 	pid;
	char	**p_path;
	char	***p_argv;
	int		*pipe_fd;

	if (argc < 3)
		return (ft_dprintf(2, "Usage: %s <program> <program>\n", argv[0]), 0);
	argv++;
	p_argv = generate_children_argv(argv);
	if (!p_argv)
		return(-1);
	p_path = generate_children_path(p_argv, envp);
	if (!p_path)
		return (free_trptr(p_argv), ft_dprintf(2, "Malloc fail :("));
	pipe_fd = malloc((argc - 1) * sizeof(void *));
	if (!pipe_fd)
		return (ft_dprintf(2, "Malloc fail (this leaks)"), -1);
	if (pipe(pipe_fd) == -1)
		return (perror("Pipe error (this also leaks)"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("Error duplicating"), -1);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		execve(p_path[0], p_argv[0], envp);
		return (perror("Error transitioning child"), -1);
	}
	pid = fork();
	if (pid == -1)
		return (perror("Error creating child"), -1);
	if (pid == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		execve(p_path[1], p_argv[1], envp);
		return (perror("Error transitioning child"), -1);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait(0);
	wait(0);
	free_dbptr(p_path);
	free_trptr(p_argv);
	free(pipe_fd);
	return (0);
}

int	ptrlen(char *content)
{
	int	ret;

	ret = 0;
	if (!content)
		return (0);
	while (*content++)
		ret++;
	return (ret);
}

void free_dbptr(char **content)
{
	char **head;

	if (!content)
		return ;
	head = content;
	while(*head)
	{
		free(*head);
		head++;
	}
	free(content);
}

void free_trptr(char ***content)
{
	char ***head;

	if (!content)
		return ;
	head = content;
	while (*head)
	{
		free_dbptr(*head);
		head++;
	}
	free(content);
}
