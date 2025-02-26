/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

char	*get_path(int i)
{
	char	*prefix;
	char	suffix[2];
	char	*path;

	prefix = "./assets/maps/map.";
	suffix[0] = i + '0';
	suffix[1] = '\0';
	path = strjoin(prefix, suffix);
	return (path);
}

static void	copy_old_map(char **new_map, char **old_map)
{
	int		i;

	i = 0;
	while (old_map && old_map[i])
	{
		new_map[i] = strdup(old_map[i]);
		free(old_map[i]);
		i++;
	}
	new_map[i] = NULL;
}

static int	expand_map_capacity(char ***map, int *map_capacity)
{
	int		new_capacity;
	char	**new_map;

	new_capacity = (*map_capacity) * 2;
	new_map = calloc(new_capacity + 1, sizeof(char *));
	if (!new_map)
	{
		return (-1);
	}
	if (*map)
	{
		copy_old_map(new_map, *map);
		free(*map);
	}
	*map_capacity = new_capacity;
	*map = new_map;
	return (0);
}

char	**get_map(char *path)
{
	char	**map;
	char	*line;
	int		map_capacity = 2;
	int		map_size = 0;
	int		fd = open(path, O_RDONLY);

	if (fd == -1)
	{
		perror("Error opening map file");
		return (NULL);
	}
	map = calloc((map_capacity + 1), sizeof(char *));
	if (!map)
	{
		close(fd);
		fprintf(stderr, "ERROR: memory allocation failed in get_map");
		return (NULL);
	}
	line = get_next_line(fd);
	while (line)
	{
		if (!(*line))
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';
		if (map_size >= map_capacity)
		{
			if (expand_map_capacity(&map, &map_capacity) == -1)
			{
				free(line);
				break ;
			}
		}
		map[map_size] = strdup(line);
		free(line);
		map_size++;
		line = get_next_line(fd);
	}
	map[map_size] = NULL;
	close(fd);
	return (map);
}
