/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hettahir <hettahir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:42:48 by hettahir          #+#    #+#             */
/*   Updated: 2024/11/23 01:43:20 by hettahir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	is_line(char *s)
{
	while (*s)
	{
		if (*s == '\n')
			return (1);
		s++;
	}
	return (0);
}

char	*read_file(int fd, char *buffer)
{
	char	*fake_buffer;
	int		i;
	char	*temp;

	i = 1;
	while (i)
	{
		fake_buffer = malloc((size_t)BUFFER_SIZE + 1);
		if (!fake_buffer)
			return (free(buffer), NULL);
		i = read(fd, fake_buffer, BUFFER_SIZE);
		if (i < 0)
			return (free(fake_buffer), free(buffer), NULL);
		fake_buffer[i] = 0;
		temp = alloc_and_free(buffer, fake_buffer);
		if (!temp)
			return (free(buffer), NULL);
		buffer = ft_strdup(temp);
		free(temp);
		if (!buffer)
			return (NULL);
		if (is_line(buffer))
			break ;
	}
	return (buffer);
}

char	*ft_get_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (free(buffer), NULL);
	line[i] = 0;
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	return (line);
}

char	*ft_next_line(char *buffer)
{
	char	*new_buffer;
	int		i;
	int		j;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	if (ft_strlen(buffer) - i == 0)
		return (free(buffer), NULL);
	new_buffer = malloc(ft_strlen(buffer) - i + 1);
	if (!new_buffer)
		return (free(buffer), NULL);
	j = 0;
	while (buffer[i])
		new_buffer[j++] = buffer[i++];
	new_buffer[j] = 0;
	return (free(buffer), new_buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_file(fd, buffer);
	if (!buffer)
		return (free(line), NULL);
	line = ft_get_line(buffer);
	buffer = ft_next_line(buffer);
	return (line);
}
