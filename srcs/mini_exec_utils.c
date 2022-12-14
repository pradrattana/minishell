/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchetana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 12:05:38 by prrattan          #+#    #+#             */
/*   Updated: 2022/09/13 20:32:28 by cchetana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipex	setup_pipe(int n_pipe)
{
	t_pipex	p;
	int		i;

	p.n_pipe = n_pipe;
	i = 0;
	while (i < p.n_pipe)
	{
		if (pipe(p.fd_end + i++ *2) < 0)
			perror("pipe");
	}
	return (p);
}

void	exec_pipe(t_command *c)
{
	if (!is_builtin(c->cmd_args[0]))
	{
		if (!c->cmd_path)
		{
			ft_putstr_fd(c->cmd_args[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			mini_free(&g_var);
			exit(127);
		}
		if (execve(c->cmd_path, c->cmd_args, g_var.env) < 0)
		{
			perror(c->cmd_args[0]);
			if (errno == 13)
				exit(126);
			exit(127);
		}
	}
	else
		exit(call_builtin(c));
}

void	close_pipe(t_pipex p)
{
	int	i;

	i = 0;
	while (i < p.n_pipe * 2)
	{
		if (close(p.fd_end[i++]) < 0)
			perror("close");
	}
}

int	wait_pipe(t_pipex p)
{
	int	status;
	int	i;

	i = 0;
	while (i < p.n_pipe + 1)
	{
		if (waitpid(p.proc[i++], &status, 0) < 0)
			perror("waitpid");
	}
	if (WIFSIGNALED(status))
		if (WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT)
			return (signal_exit_child(WTERMSIG(status)));
	return (WEXITSTATUS(status));
}
