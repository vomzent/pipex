/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   children_helpers.c                                :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/09/28 18:02:38 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/28 18:28:41 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "pipex.h"

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
