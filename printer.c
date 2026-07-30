/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   printer.c                                         :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/07/26 11:55:26 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/07/26 12:00:58 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */
// It prints!!
#include "Libft/libft.h"

// int main(int argc, char **argv, char **envp)
// {
// 	char **i;
//
// 	(void)argc;
// 	(void)argv;
// 	i = envp;
// 	while (*i)
// 		ft_printf("%s\n", *i++);
// 	return(0);
// }

int main(int argc, char **argv, char **envp)
{
	char **i;

	(void)argc;
	(void)envp;
	i = argv;
	while (*i)
		ft_printf("%s\n", *i++);
	return(0);
}
