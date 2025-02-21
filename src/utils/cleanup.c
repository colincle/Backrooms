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

void	cleanup_textures(t_game *game)
{
	if (game->textures.wall.texture)
	{
		SDL_DestroyTexture(game->textures.wall.texture);
		game->textures.wall.texture = NULL;
	}
	if (game->textures.ceiling.texture)
	{
		SDL_DestroyTexture(game->textures.ceiling.texture);
		game->textures.ceiling.texture = NULL;
	}
	if (game->textures.floor.texture)
	{
		SDL_DestroyTexture(game->textures.floor.texture);
		game->textures.floor.texture = NULL;
	}
	if (game->textures.screen_texture)
	{
		SDL_DestroyTexture(game->textures.screen_texture);
		game->textures.screen_texture = NULL;
	}
}


void	cleanup_sounds(t_game *game)
{
	if (SOUNDS.ambient)
		Mix_FreeMusic(SOUNDS.ambient);
	if (SOUNDS.running)
		Mix_FreeChunk(SOUNDS.running);
	if (SOUNDS.walking)
		Mix_FreeChunk(SOUNDS.walking);
	Mix_CloseAudio();
}

void	cleanup(t_game *game)
{
	cleanup_sounds(game);
	cleanup_textures(game);
	free_all_maps(game);
	free_entities(game);
	free_vector_grid(game);
	free(game);
	quit_game(game);
}
