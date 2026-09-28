/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                             :+:    :+:           */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/21 10:06:24 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/28 18:27:33 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_program	*programs;
	int			**pipes;

	if (argc < 3)
		return (ft_dprintf(2, "Usage: %s <program> <program>\n", argv[0]), 0);
	programs = ft_calloc(argc, sizeof(t_program));
	pipes = create_pipes(argc - 2);
	if (!programs || !pipes)
		return (cleanup_program(programs, pipes), -1);
	if (generate_children_argv(argv, programs))
		return (cleanup_program(programs, pipes), -1);
	assign_pipes(programs, pipes);
	programs[0].in_fd = 0;
	programs[argc - 1].out_fd = 1;
	run_programs(programs, pipes, envp);
	close_pipes(pipes);
	await_programs(programs);
	cleanup_program(programs, pipes);
	return (0);
}

