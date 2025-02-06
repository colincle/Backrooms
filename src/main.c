/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	quit_game(t_game *game)
{
	SDL_DestroyWindow(WINDOW);
	SDL_Quit();
	cleanup(game);
}

void frame_sync(t_game *game)
{
	FRAME_TIME = SDL_GetTicks() - FRAME_START;
	if (FRAME_TIME < FRAME_DELAY)
	{
		SDL_Delay((Uint32)(FRAME_DELAY - FRAME_TIME));
		while ((SDL_GetTicks() - FRAME_START) < FRAME_DELAY);
	}
	FPS = (FRAME_TIME > 0) ? (1000.0f / FRAME_TIME) : 1000.0f;
	fflush(stdout);
}


void	game_loop(t_game *game)
{
	int		running;
	void (*chapter[])(t_game *game, int *i, int *running) = {chapter_1, chapter_2, chapter_3, chapter_4, chapter_5};
	running = TRUE;
	int		i = 0;

	while (running)
	{
		FRAME_START = SDL_GetTicks();
		chapter[i](game, &i, &running);
		handle_events(game, &running);
		//render_next_frame(game); to do
		frame_sync(game);
	}
}

int	main(void)
{
	t_game	*game;

	game = game_init();
	game_loop(game);
	quit_game(game);
	return (0);
}

/*
TO DO
- Copy maps into array
- Print minimap
- Handle collisions
- Raycast
*/