/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.h                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/26 12:18:20 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/28 15:49:32 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <sys/types.h>
# include <unistd.h>

typedef struct s_program
{
	pid_t	pid;
	char	*name;
	char	**args;
	int		in_fd;
	int		out_fd;
}			t_program;

#endif
