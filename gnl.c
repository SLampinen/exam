/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slampine <slampine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 11:07:24 by slampine          #+#    #+#             */
/*   Updated: 2023/10/12 09:38:45 by slampine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif
static char	*get_unhandled(char *text, int *error)
{
	char	*unhandled;
	size_t	line_len;
	size_t	text_len;

	line_len = 0;
	text_len = ft_strlen(text);
	while (text[line_len] != '\0' && text[line_len] != '\n')
		line_len++;
	if (text[line_len] == '\0')
		return (0);
	unhandled = ft_substr(text, line_len + 1, text_len - line_len);
	if (*unhandled == '\0')
	{
		free(unhandled);
		unhandled = NULL;
		if (text_len - line_len - 1 != 0)
			*error = 1;
	}
	text[line_len + 1] = '\0';
	return (unhandled);
}

static char	*read_until(int fd, char *buffer, char *stash)
{
	char	*temp;
	int		read_bytes;

	if (stash != NULL && (ft_strchr(stash, '\n')))
		return (stash);
	while ((stash && !ft_strchr(stash, '\n')) || !stash)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == 0)
			break ;
		if (read_bytes == -1)
			return (NULL);
		if (!stash)
			stash = ft_strdup("");
		if (!stash)
			return (NULL);
		buffer[read_bytes] = '\0';
		temp = stash;
		stash = ft_strjoin(temp, buffer);
		if (!stash)
			return (NULL);
		free(temp);
		temp = NULL;
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*unhandled[OPEN_MAX];
	char		*buffer;
	char		*current_line;
	int			error;

	error = 0;
	if (BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (NULL);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char *));
	if (!buffer)
		return (NULL);
	current_line = read_until(fd, buffer, unhandled[fd]);
	free(buffer);
	if (!current_line)
	{
		free(unhandled[fd]);
		return (NULL);
	}
	unhandled[fd] = get_unhandled(current_line, &error);
	if (error)
	{
		free(current_line);
		return (NULL);
	}
	return (current_line);
}
char	*gnl_short(int fd)
{
	int		index;
	int		bytes;
	char	*buffer;
	char	character;

	if ((fd < 0) || (BUFFER_SIZE <= 0))
		return (NULL);
	index = 0;
	bytes = read(fd, &character, 1);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	while (bytes > 0)
	{
		buffer[index++] = character;
		if (character == '\n')
			break ;
		bytes = read(fd, &character, 1);
	}
	if ((bytes <= 0) && (index == 0))
		return (free(buffer), NULL);
	buffer[index] = '\0';
	return (buffer);

}
