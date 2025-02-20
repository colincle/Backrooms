/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	quit_game(t_game *game)
{
	SDL_DestroyWindow(WINDOW);
	SDL_DestroyRenderer(RENDERER);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}

void map_trigger(t_game *game)
{
	if (MAPS[LEVEL][(int)PLAYER_Y][(int)PLAYER_X] == TRIGGER)
	{
		game->player[LEVEL + 1]->x = PLAYER_X;
		game->player[LEVEL + 1]->y = PLAYER_Y;
		game->player[LEVEL + 1]->dir.x = PLAYER_DIR_X;
		game->player[LEVEL + 1]->dir.y = PLAYER_DIR_Y;
		LEVEL++;
	}
}

void sounds(t_game *game)
{
	if (!Mix_PlayingMusic())
	{
		if (Mix_PlayMusic(SOUNDS.ambient, -1) == -1)
		{
			fprintf(stderr, "Failed to play music: %s\n", Mix_GetError());
		}
	}
	if (MOVING == RUNNING)
	{
		if (Mix_Playing(2))
			Mix_HaltChannel(2);
		if (!Mix_Playing(1))
		{
			Mix_PlayChannel(1, SOUNDS.running, 0);
		}
	}
	if (MOVING == WALKING)
	{
		if (Mix_Playing(1))
			Mix_HaltChannel(1);
		if (!Mix_Playing(2))
		{
			Mix_PlayChannel(2, SOUNDS.walking, 0);
		}
	}
	if (MOVING == STILL)
	{
		Mix_HaltChannel(-1);
	}
}


static void	game_loop(t_game *game)
{
	int		running;
	void	(*chapter[])(t_game *game, int *running) = {chapter_1, chapter_2, chapter_3, chapter_4, chapter_5};

	running = TRUE;
	LEVEL = START_LEVEL;
	while (running)
	{
		sounds(game);
		chapter[LEVEL](game, &running);
		map_trigger(game);
		handle_events(game, &running);
		manage_controller(game);
		update_entities(game);
		render_next_frame(game);
		debug_statements(game);
		manage_fps(game);
	}
}

int	main(void)
{
	t_game	*game;

	game = game_init();
	game_loop(game);
	cleanup(game);
	return (0);
}
