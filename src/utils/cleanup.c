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

void	cleanup_textures(t_game *game)
{
	if (game->textures.wall.texture)
	{
		SDL_DestroyTexture(game->textures.wall.texture);
		game->textures.wall.texture = NULL;
	}
	if (game->textures.wall_dark.texture)
	{
		SDL_DestroyTexture(game->textures.wall_dark.texture);
		game->textures.wall_dark.texture = NULL;
	}
	if (game->textures.ceiling.texture)
	{
		SDL_DestroyTexture(game->textures.ceiling.texture);
		game->textures.ceiling.texture = NULL;
	}
	if (game->textures.ceiling_dark.texture)
	{
		SDL_DestroyTexture(game->textures.ceiling_dark.texture);
		game->textures.ceiling_dark.texture = NULL;
	}
	if (game->textures.ceiling_darker.texture)
	{
		SDL_DestroyTexture(game->textures.ceiling_darker.texture);
		game->textures.ceiling_darker.texture = NULL;
	}
	if (game->textures.floor.texture)
	{
		SDL_DestroyTexture(game->textures.floor.texture);
		game->textures.floor.texture = NULL;
	}
	if (game->textures.floor_light.texture)
	{
		SDL_DestroyTexture(game->textures.floor_light.texture);
		game->textures.floor_light.texture = NULL;
	}
	if (game->textures.floor_lighter.texture)
	{
		SDL_DestroyTexture(game->textures.floor_lighter.texture);
		game->textures.floor_lighter.texture = NULL;
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
	free(game->z_buffer);
	free(game->screen);
	free_entities(game);
	free(game);
	quit_game(game);
}
