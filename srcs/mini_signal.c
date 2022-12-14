/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchetana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 19:50:48 by cchetana          #+#    #+#             */
/*   Updated: 2022/09/13 20:24:24 by cchetana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	echo_off(void)
{
	struct termios	tio;

	tcgetattr(STDIN_FILENO, &tio);
	tio.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &tio);
}

static void	respond_sig(int signum, siginfo_t *info, void *context)
{
	(void) context;
	(void) info;
	if (signum == SIGINT)
	{
		g_var.sig_detect = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else if (signum == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signal_settings(void)
{
	struct sigaction	sact;

	if (g_var.sig_detect)
		g_var.exit_status = EXIT_SIGINT;
	g_var.sig_detect = 0;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = SA_SIGINFO;
	sact.sa_sigaction = respond_sig;
	sigaction(SIGINT, &sact, NULL);
	sigaction(SIGQUIT, &sact, NULL);
	echo_off();
}

void	signal_settings_child(int mode)
{
	struct sigaction	sact;

	if (mode == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (mode == 1)
	{
		signal(SIGINT, SIG_DFL);
		sigemptyset(&sact.sa_mask);
		sact.sa_flags = SA_SIGINFO;
		sact.sa_sigaction = respond_sig;
		sigaction(SIGQUIT, &sact, NULL);
	}
}

int	signal_exit_child(int signum)
{
	if (signum == SIGINT)
		return (EXIT_SIGINT);
	return (EXIT_SIGQUIT);
}
