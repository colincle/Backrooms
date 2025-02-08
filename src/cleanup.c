/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	free_entities(t_game *game)
{
	int	i;
	int	e;

	if (!game)
		return ;
	if (game->player)
	{
		i = 0;
		while (i < NUMBER_OF_MAPS)
			free(game->player[i++]);
		free(game->player);
		game->player = NULL;
	}
	if (game->enemy)
	{
		i = 0;
		while (i < NUMBER_OF_MAPS)
		{
			if (game->enemy[i])
			{
				e = 0;
				while (game->enemy[i][e])
					free(game->enemy[i][e++]);
				free(game->enemy[i]);
			}
			i++;
		}
		free(game->enemy);
		game->enemy = NULL;
	}
}

void	free_all(char **array)
{
	int		i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_all_maps(t_game *game)
{
	int		i;

	i = 0;
	if (!game->maps)
		return ;
	while (game->maps[i])
	{
		free_all(game->maps[i]);
		i++;
	}
	free(game->maps);
	game->maps = NULL;
}

void	cleanup(t_game *game)
{
	if (game->frames)
		free(game->frames);
	free_all_maps(game);
	free_entities(game);
	free(game);
}
