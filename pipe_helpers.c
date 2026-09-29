/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipe_helpers.c                                    :+:    :+:             */
/*                                                    +:+                     */
/*   By: vcoevert <vcoevert@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2026/09/28 17:58:56 by vcoevert     #+#    #+#                  */
/*   Updated: 2026/09/29 11:46:20 by vcoevert     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "pipex.h"

int	**create_pipes(int amount)
{
	int	**ret;
	int	i;

	ret = ft_calloc(amount + 1, sizeof(int *));
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

void	assign_pipes(t_program *programs, int **pipes)
{
	int	i;
	int	pipelen;

	i = 0;
	pipelen = ptrlen((char **)pipes);
	while (i < pipelen)
	{
		programs[i].out_fd = pipes[i][1];
		programs[i + 1].in_fd = pipes[i][0];
		i++;
	}
}

void	close_pipes(int **pipes)
{
	int	i;
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
