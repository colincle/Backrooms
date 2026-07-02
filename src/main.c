#include <SDLRaycaster.h>

void	quit_game(t_game *game)
{
	SDL_DestroyWindow(WINDOW);
	SDL_DestroyRenderer(RENDERER);
	SDL_Quit();
	exit(EXIT_SUCCESS);
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
		level_trigger(game);
		handle_events(game, &running);
		update_entities(game);
		render_next_frame(game);
		debug_statements(game);
		manage_fps(game);
	}
}

int	main(void)
{
	t_game	*game;

	game = init_game();
	game_loop(game);
	cleanup(game);
	return (0);
}
