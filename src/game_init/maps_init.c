#include <SDLRaycaster.h>

t_point	get_map_size(char **map)
{
	int		x;
	t_point	sizes;

	x = 0;
	sizes.x = 0;
	sizes.y = 0;
	while (map[sizes.y])
	{
		x = 0;
		while (map[sizes.y][x])
		{
			if (x > sizes.x)
				sizes.x = x;
			x++;
		}
		sizes.y++;
	}
	sizes.y--;
	return (sizes);
}

void	maps_init(t_game *game)
{
	char	*path;
	int		i = 1;

	MAPS = malloc(sizeof(char **) * (NUMBER_OF_MAPS + 1));
	if (!MAPS)
	{
		fprintf(stderr, "ERROR: memory allocation failed in maps_init");
		cleanup(game);
	}
	game->maps_sizes = malloc(sizeof(t_point) * (NUMBER_OF_MAPS));
	if (!MAPS)
	{
		fprintf(stderr, "ERROR: memory allocation failed in maps_init");
		cleanup(game);
	}
	while (i <= NUMBER_OF_MAPS)
	{
		path = get_path(i);
		MAPS[i - 1] = get_map(path);
		if (!MAPS[i - 1])
			cleanup(game);
		game->maps_sizes[i - 1] = get_map_size(MAPS[i - 1]);
		free(path);
		i++;
	}
	MAPS[NUMBER_OF_MAPS] = NULL;
}
