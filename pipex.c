/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                             :+:    :+:           */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/24 20:30:28 by vcoevert     ########   odam.nl          */
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
	int		i;
	int		j;

	if (argc < 3)
		return (ft_dprintf(2, "Usage: %s <program> <program>\n", argv[0]), 0);
	argv++;
	i = 0;
	p_argv = generate_children_argv(argv);
	if (!p_argv)
		return(-1);
	p_path = generate_children_path(p_argv, envp);
	if (!p_path)
		return (free_trptr(p_argv), ft_dprintf(2, "Malloc fail :("));
	pipe_fd = malloc((argc - 2) * sizeof(int) * 2);
	if (!pipe_fd)
		return (ft_dprintf(2, "Malloc fail (this leaks)"), -1);
	while (i < argc - 2)
		if (pipe(&pipe_fd[i++ * 2]) == -1)
			return (perror("Pipe error (this also leaks)"), -1);
	i = 0;
	while (i < argc - 1)
	{
		pid = fork();
		if (pid == -1)
			return (perror("Error duplicating"), -1);
		if (pid == 0)
		{
			if (i != argc - 1)
			{
				close(pipe_fd[i * 2]); //close read end of this pipe
				dup2(pipe_fd[i * 2 + 1], 1); 
				close(pipe_fd[i * 2 + 1]);
			}
			if (i != 0)
			{
				close(pipe_fd[i * 2 - 2 + 1]);
				dup2(pipe_fd[i * 2 - 2], 0);
				close(pipe_fd[i * 2 - 2]);
			}
			j = 0;
			while (j < argc - 2)
			{
				if ((i == 0 || j != i - 1) && (i == argc - 1 || j != i))
				{
					close(pipe_fd[i * 2]);
					close(pipe_fd[i * 2 + 1]);
				}
				j++;
			}
			execve(p_path[i], p_argv[i], envp);
			perror("Error transitioning child");
			return (-1);
		}
		i++;
	}
	i = 0;
	while (i < argc -2)
	{
		close(pipe_fd[i*2]);
		close(pipe_fd[i*2+1]);
		i++;
	}
	i = 0;
	while (i++ < argc - 1)
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
