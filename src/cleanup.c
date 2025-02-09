/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	free_entities(t_game *game)
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

static void	free_all_maps(t_game *game)
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

static void	free_vector_grid(t_game *game)
{
	int	i;
	int	y;

	if (!game || !game->vector_grid)
		return ;
	i = 0;
	while (game->vector_grid[i])
	{
		y = 0;
		while (game->vector_grid[i][y])
			free(game->vector_grid[i][y++]);
		free(game->vector_grid[i]);
		i++;
	}
	free(game->vector_grid);
	game->vector_grid = NULL;
}

void	cleanup(t_game *game)
{
	free_all_maps(game);
	free_entities(game);
	free_vector_grid(game);
	free(game);
}
