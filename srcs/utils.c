#include "minishell.h"

int	ft_wcount(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			++s;
		if (*s != c && *s)
			++count;
		while (*s != c && *s)
			++s;
	}
	return (count);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}

int	ft_isspecial(char c)
{
	if (c == '-')
		return ('-');
	// if (c == '\\')
	// 	return ('\\');
	// if (c == '/')
	// 	return ('s');
	if (c == '>')
		return ('>');
	if (c == '<')
		return ('<');
	if (c == '|')
		return ('|');
	return (ft_isspace(c));
}