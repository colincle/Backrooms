#include <SDLRaycaster.h>

void	quit_game(t_game *game)
{
	SDL_DestroyWindow(WINDOW);
	SDL_DestroyRenderer(RENDERER);
	SDL_Quit();
}

static void	level_trigger(t_game *game)
{
	if (MAPS[LEVEL][(int)PLAYER_Y][(int)PLAYER_X] == TRIGGER)
	{
		game->player[LEVEL + 1]->x = PLAYER_X;
		game->player[LEVEL + 1]->y = PLAYER_Y;
		game->player[LEVEL + 1]->dir.x = PLAYER_DIR_X;
		game->player[LEVEL + 1]->dir.y = PLAYER_DIR_Y;
		game->player[LEVEL + 1]->cam.x = PLAYER_CAM_X;
		game->player[LEVEL + 1]->cam.y = PLAYER_CAM_Y;
		game->player[LEVEL + 1]->camera_shift = CAM_SHIFT;
		game->player[LEVEL + 1]->feet_touch = FEET_TOUCH;
		game->player[LEVEL + 1]->jumping = JUMP;
		game->player[LEVEL + 1]->crouching = CROUCH;
		game->player[LEVEL + 1]->speed = PLAYER_SPEED;
		game->player[LEVEL + 1]->crouch_lock = CROUCH_LOCK;
		game->player[LEVEL + 1]->stand_lock = STAND_LOCK;
		game->player[LEVEL + 1]->falling = FALLING;
		game->player[LEVEL + 1]->jump_lock = JUMP_LOCK;
		LEVEL++;
	}
}

static void	game_frame(void *arg)
{
	static void	(*chapter[])(t_game *game, int *running) = {chapter_1, chapter_2, chapter_3, chapter_4, chapter_5};
	t_main_loop	*loop = arg;
	t_game		*game = loop->game;

	sounds(game);
	chapter[LEVEL](game, &loop->running);
	level_trigger(game);
	handle_events(game, &loop->running);
	update_entities(game);
	render_next_frame(game);
	debug_statements(game);
	manage_fps(game);
#ifdef __EMSCRIPTEN__
	if (!loop->running)
		emscripten_cancel_main_loop();
#endif
}

static void	game_loop(t_game *game)
{
	// static: under Emscripten this function returns while frames keep firing
	static t_main_loop	loop;

	loop.game = game;
	loop.running = TRUE;
	LEVEL = START_LEVEL;
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(game_frame, &loop, 0, TRUE);
#else
	while (loop.running)
		game_frame(&loop);
#endif
}

int	main(void)
{
	t_game	*game;

	game = init_game();
	game_loop(game);
	cleanup(game);
	return (0);
}
