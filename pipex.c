/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                             :+:    :+:           */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/28 20:09:18 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_program	*programs;
	int			**pipes;
	int			in_out_fd[2];

	if (argc < 5)
		return (ft_dprintf(2, "Usage: %s infile <program> <program> outfile\n", argv[0]), 0);
	in_out_fd[0] = open(argv[1], O_RDONLY);
	if (in_out_fd[0] == -1)
		return (perror("Error opening input file"), -1);
	in_out_fd[1] = open(argv[argc - 1], O_WRONLY + O_CREAT, 0644);
	if (in_out_fd[1] == -1)
		return (perror("Error opening output file"), -1);
	programs = ft_calloc(argc - 2, sizeof(t_program));
	pipes = create_pipes(argc - 4);
	if (!programs || !pipes)
		return (cleanup_program(programs, pipes), -1);
	if (generate_children_argv(argv, programs))
		return (cleanup_program(programs, pipes), -1);
	assign_pipes(programs, pipes);
	programs[0].in_fd = in_out_fd[0];
	programs[argc - 4].out_fd = in_out_fd[1];
	run_programs(programs, pipes, envp);
	close_pipes(pipes);
	await_programs(programs);
	cleanup_program(programs, pipes);
	return (0);
}

