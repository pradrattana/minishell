/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_input_checker.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchetana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 19:48:52 by cchetana          #+#    #+#             */
/*   Updated: 2022/09/13 19:48:53 by cchetana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_quote_valid(t_shell *sh)
{
	int	i;
	int	curr_q;

	i = 0;
	curr_q = 0;
	while (sh->from_rl[i] && !ft_isquotes(sh->from_rl[i]))
		i++;
	while (sh->from_rl[i])
	{
		if (ft_isquotes(sh->from_rl[i]))
		{
			curr_q = sh->from_rl[i++];
			i += pos_in_str(sh->from_rl + i, curr_q);
		}
		if (sh->from_rl[i++] == curr_q)
			curr_q = 0;
	}
	if (curr_q)
	{
		ft_putstr_fd("tinyshell: "RED"invaid quotes\n"RES, STDERR_FILENO);
		sh->exit_status = EXIT_FAILURE;
		return (0);
	}
	return (1);
}

static int	is_pipe_valid(char *from_rl)
{
	char	**temp;
	int		i;

	if (count_in_str(from_rl, '|') > 0)
	{
		if (*from_rl == '|' || !*(ft_strchr(from_rl, '|') + 1))
		{
			put_error(NULL, "|", 2);
			return (0);
		}
		temp = ft_split(from_rl, '|');
		i = 0;
		while (temp[i])
		{
			if (ft_isspace_str(temp[i++]))
			{
				put_error(NULL, "|", 2);
				free_2d(temp);
				return (0);
			}
		}
		free_2d(temp);
	}
	return (1);
}

static int	redir_put_error(char *str)
{
	if (!*str)
		put_error(NULL, "newline", 2);
	else
	{
		*(str + 1) = 0;
		put_error(NULL, str, 2);
	}
	return (0);
}

static int	is_redir_valid(char *from_rl)
{
	char	*set;
	char	*str;

	set = "><";
	if (pos_in_str(from_rl, '<') < pos_in_str(from_rl, '>'))
		set = "<>";
	while (*set)
	{
		str = ft_strchr(from_rl, *set);
		while (str)
		{
			if (*++str == *set)
				str++;
			while (*str && ft_isspace(*str))
				str++;
			if (!*str || *str == '|' || count_in_str("><", *str) > 0)
				return (redir_put_error(str));
			str = ft_strchr(str, *set);
		}
		set++;
	}
	return (1);
}

int	is_input_valid(t_shell *sh)
{
	if (is_quote_valid(sh))
	{
		if (is_pipe_valid(sh->from_rl))
		{
			if (is_redir_valid(sh->from_rl))
				return (1);
		}
	}
	return (0);
}
