/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.h                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/26 12:18:20 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/28 18:35:59 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_program
{
	pid_t	pid;
	char	*name;
	char	**args;
	int		in_fd;
	int		out_fd;
}			t_program;

int		**create_pipes(int amount);
void	assign_pipes(t_program *program, int **pipes);
void	close_pipes(int **pipes);

char	*find_program_path(char *name, char **envp);
int		generate_children_argv(char **argv, t_program * programs);
void	run_programs(t_program *programs, int **pipes, char **envp);
void	await_programs(t_program *programs);

void	cleanup_program(t_program *programs, int **pipes);
int		ptrlen(char	**content);
void	free_dbptr(char **content);
#endif
