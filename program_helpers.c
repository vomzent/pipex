/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   program_helpers.c                                 :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/09/28 18:04:26 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/28 18:28:21 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "pipex.h"

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
