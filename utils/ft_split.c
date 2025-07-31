/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hettahir <hettahir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 03:48:05 by hettahir          #+#    #+#             */
/*   Updated: 2024/10/30 12:01:59 by hettahir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int	ft_cont_word(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || *(s + 1) == '\0'))
			count++;
		s++;
	}
	return (count);
}

int	ft_word_len(char const *s, char c, int i)
{
	int	len;

	len = 0;
	while (s[i])
	{
		if (s[i] == c)
			break ;
		i++;
		len++;
	}
	return (len);
}

char	*ft_copy_word(char const *s, char c, int *i)
{
	int		len;
	int		j;
	char	*str;

	j = 0;
	while (s[*i])
	{
		if (s[*i] == c)
			(*i)++;
		else
			break ;
	}
	len = ft_word_len(s, c, *i);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (j < len)
	{
		str[j] = s[*i];
		j++;
		(*i)++;
	}
	str[j] = 0;
	return (str);
}

void	ft_free(char **strs, int i)
{
	while (i >= 0)
		free(strs[i--]);
	free(strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		words;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	j = 0;
	i = 0;
	words = ft_cont_word(s, c);
	strs = (char **)malloc((words + 1) * sizeof(char *));
	if (!strs)
		return (NULL);
	while (j < words)
	{
		strs[j] = ft_copy_word(s, c, &i);
		if (!strs[j])
		{
			ft_free(strs, j);
			return (NULL);
		}
		j++;
	}
	strs[j] = 0;
	return (strs);
}
