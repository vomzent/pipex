/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.h                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/26 12:18:20 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/07/27 11:21:40 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typdef struct s_program
{
	char	*p_name;
	char	**p_args;
}			t_program;

typdef struct s_pipex
{
	char	*infile;
	t_list	*programs;
	char	*outfile;
}

#endif
