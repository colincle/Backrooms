#include <SDLRaycaster.h>

// Emscripten's limits.h does not define OPEN_MAX
#ifndef OPEN_MAX
# define OPEN_MAX 256
#endif

static char	*read_from_file(int fd, char *leftover)
{
	char		*buffer;
	long long	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
	{
		fprintf(stderr, "ERROR: memory allocation failed in read_from_file");
		exit(EXIT_FAILURE);
	}
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
	if (!str)
	{
		fprintf(stderr, "ERROR: memory allocation failed in find_line");
		exit(EXIT_FAILURE);
	}
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
