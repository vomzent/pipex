/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                             :+:    :+:           */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/28 16:45:43 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pipex.h"

void free_dbptr(char **content);
// void free_trptr(char ***content);
int	ptrlen(char **content);
void	cleanup_program(t_program *programs, int **pipes);

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
	while(envp && *path)
	{
		ret = ft_strjoin(*path, name);
		if (!ret || !access(ret, X_OK))
			return (free_dbptr(envp), free(name), ret);
		free(ret);
		path++;
	}
	ft_dprintf(2, "Program not found ;(");
	return (free_dbptr(envp), free(name), (char *)0);
}

int	generate_children_argv(char **argv, t_program *programs)
{
	int i;

	i = 0;
	while (i < ptrlen(argv) - 1)
	{
		programs[i].args = ft_split(argv[i + 1], ' ');
		if (!programs[i].args)
			return (ft_dprintf(2, "Malloc fail"), -1);
		i++;
	}
	return (0);
}

// char **generate_children_path(char ***p_argv, char **envp)
// {
// 	char	**ret;
// 	char	**p_path;
//
// 	ret = malloc((ptrlen((char **)p_argv) + 1) * sizeof(void *));
// 	p_path = ret;
// 	if (!p_path)
// 		return (ret);
// 	while (*p_argv)
// 	{
// 		*p_path = find_program_path(**p_argv, envp);
// 		if (!*p_path)
// 		{
// 			free_dbptr(ret);
// 			return (0);
// 		};
// 		p_argv++;
// 		p_path++;
// 	}
// 	*p_path = 0;
// 	return (ret);
// }

int	**create_pipes(int amount)
{
	int	**ret;
	int	i;

	ret = calloc(amount + 1, sizeof(int *));
	if (!ret)
		return (ret);
	i = 0;
	while (i < amount)
	{
		ret[i] = malloc(sizeof(int) * 2);
		if (!ret[i] || pipe(ret[i]) == -1)
		{
			free_dbptr((char **)ret);
			ft_dprintf(2, "Malloc fail");
			return (0);
		}
		i++;
	}
	return (ret);
}

void asssign_pipes(t_program *programs, int **pipes)
{
	int	i;
	int pipelen;

	i = 0;
	pipelen = ptrlen((char **)pipes);
	while (i < pipelen)
	{
		programs[i].out_fd = pipes[i][1];
		programs[i + 1].in_fd = pipes[i][0];
		i++;
	}
}

void close_pipes(int **pipes)
{
	int i;
	int	pipelen;

	i = 0;
	pipelen = ptrlen((char **)pipes);
	while (i < pipelen)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}	

void run_programs(t_program *programs, int **pipes, char **envp)
{
	while(programs->args)
	{
		programs->pid = fork();
		if (programs->pid == -1)
		{
			perror("Error making child");
			return ;
		}
		if (programs->pid == 0)
		{
			if (programs->out_fd != 1)
				dup2(programs->out_fd, 1);
			if (programs->in_fd != 0)
				dup2(programs->in_fd, 0);
			close_pipes(pipes);
			programs->name = find_program_path(programs->args[0], envp);
			if (!programs->name)
				break;
			execve(programs->name, programs->args, envp);
			perror("error transitioning child");
			return ;
		}
		programs++;
	}
}

void await_programs(t_program *programs)
{
	while (programs->args)
	{
		wait(&programs->pid);
		programs++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_program	*programs;
	int			**pipes;

	if (argc < 3)
		return (ft_dprintf(2, "Usage: %s <program> <program>\n", argv[0]), 0);
	programs = calloc(argc, sizeof(t_program));
	pipes = create_pipes(argc - 2);
	if (!programs || !pipes)
		return (cleanup_program(programs, pipes), -1);
	if (generate_children_argv(argv, programs))
		return (cleanup_program(programs, pipes), -1);
	asssign_pipes(programs, pipes);
	programs[0].in_fd = 0;
	programs[argc - 1].out_fd = 1;
	run_programs(programs, pipes, envp);
	close_pipes(pipes);
	await_programs(programs);
	cleanup_program(programs, pipes);
	return (0);
}

void cleanup_program(t_program *programs, int **pipes)
{
	t_program *head;

	head = programs;
	while (head && head->args)
	{
		if (head->pid > 0)
			wait(&head->pid);
		if (head->name)
			free(head->name);
		if (head->args)
			free_dbptr(head->args);
		head++;
	}
	if (programs)
		free(programs);
	free_dbptr((char **)pipes);
}

int	ptrlen(char **content)
{
	int		ret;

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

// void free_trptr(char ***content)
// {
// 	char ***head;
//
// 	if (!content)
// 		return ;
// 	head = content;
// 	while (*head)
// 	{
// 		free_dbptr(*head);
// 		head++;
// 	}
// 	free(content);
// }
