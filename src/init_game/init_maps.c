/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	init_maps(t_game *game)
{
	char	*path;
	int		i = 1;

	MAPS = malloc(sizeof(char **) * (NUMBER_OF_MAPS + 1));
	if (!MAPS)
	{
		fprintf(stderr, "ERROR: memory allocation failed in init_maps");
		exit(EXIT_FAILURE);
	}
	while (i <= NUMBER_OF_MAPS)
	{
		path = get_path(i);
		MAPS[i - 1] = get_map(path);
		if (!MAPS[i - 1])
			cleanup(game);
		free(path);
		if (!MAPS[i - 1])
		{
			free(MAPS);
			MAPS = NULL;
			return ;
		}
		i++;
	}
	MAPS[NUMBER_OF_MAPS] = NULL;
}

