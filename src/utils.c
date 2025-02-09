/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

char	*strjoin(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	len1 = strlen(s1);
	len2 = strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	strcpy(result, s1);
	strcat(result, s2);
	return (result);
}

static char	*read_from_file(int fd, char *leftover)
{
	char		*buffer;
	long long	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	memset(buffer, 0, BUFFER_SIZE);
	while (!strchr(buffer, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(leftover);
			free(buffer);
			return (NULL);
		}
		else if (bytes_read == 0)
			break ;
		else
			buffer[bytes_read] = '\0';
		leftover = strjoin(leftover, buffer);
	}
	free(buffer);
	return (leftover);
}

static char	*find_line(char *leftover)
{
	char	*str;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (leftover[i] != '\n' && leftover[i])
		i++;
	str = malloc(sizeof(char) * (i + 2));
	while (i > j)
	{
		str[j] = leftover[j];
		j++;
	}
	if (leftover[j] == '\n')
	{
		str[j] = leftover[j];
		j++;
	}
	str[j] = '\0';
	return (str);
}

static char	*find_leftover(char *leftover)
{
	while (*leftover && *leftover != '\n')
		leftover++;
	if (*leftover == '\n')
		leftover++;
	if (*leftover == '\0')
		return (NULL);
	return (strdup(leftover));
}

char	*get_next_line(int fd)
{
	static char	*leftover[OPEN_MAX];
	char		*next_line;
	char		*temp;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	leftover[fd] = read_from_file(fd, leftover[fd]);
	if (!leftover[fd])
	{
		free(leftover[fd]);
		return (NULL);
	}
	next_line = find_line(leftover[fd]);
	temp = leftover[fd];
	leftover[fd] = find_leftover(leftover[fd]);
	if (leftover[fd] == NULL)
		free(leftover[fd]);
	free(temp);
	return (next_line);
}
