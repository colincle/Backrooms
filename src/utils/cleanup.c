/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	free_entities(t_game *game)
{
	int	i;
	int	e;

	if (!game || !game->player)
		return ;
	i = 0;
	while (i < NUMBER_OF_MAPS)
	{
		if (game->player[i])
		{
			free(game->player[i]);
			game->player[i] = NULL;
		}
		i++;
	}
	free(game->player);
	game->player = NULL;
	if (!game->enemy)
		return ;
	i = 0;
	while (i < NUMBER_OF_MAPS)
	{
		if (game->enemy[i])
		{
			e = 0;
			while (game->enemy[i][e])
			{
				free(game->enemy[i][e]);
				game->enemy[i][e] = NULL;
				e++;
			}
			free(game->enemy[i]);
			game->enemy[i] = NULL;
		}
		i++;
	}
	free(game->enemy);
	game->enemy = NULL;
}

static void	free_all_maps(t_game *game)
{
	int	i;

	if (!game || !game->maps)
		return ;
	i = 0;
	while (game->maps[i])
	{
		free_all(game->maps[i]);
		game->maps[i] = NULL;
		i++;
	}
	free(game->maps);
	game->maps = NULL;
}

static void	cleanup_textures(t_game *game)
{
	if (!game)
		return ;
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

static void	cleanup_sounds(t_game *game)
{
	if (!game)
		return ;
	if (SOUNDS.ambient)
	{
		Mix_FreeMusic(SOUNDS.ambient);
		SOUNDS.ambient = NULL;
	}
	if (SOUNDS.running)
	{
		Mix_FreeChunk(SOUNDS.running);
		SOUNDS.running = NULL;
	}
	if (SOUNDS.walking)
	{
		Mix_FreeChunk(SOUNDS.walking);
		SOUNDS.walking = NULL;
	}
	Mix_CloseAudio();
}

void	cleanup(t_game *game)
{
	if (!game)
		return ;
	cleanup_sounds(game);
	cleanup_textures(game);
	free_all_maps(game);
	if (game->z_buffer)
	{
		free(game->z_buffer);
		game->z_buffer = NULL;
	}
	if (game->screen)
	{
		free(game->screen);
		game->screen = NULL;
	}
	free_entities(game);
	free(game);
	quit_game(game);
}
