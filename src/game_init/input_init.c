#include <SDLRaycaster.h>

void	keyboard_key_states_init(t_game *game)
{
	KEYS = calloc(HOW_MANY_KEYS, sizeof(int));
	if (!KEYS)
	{
		fprintf(stderr, "ERROR: memory allocation failed in game_struct_init");
		cleanup(game);
	}
}

void	controller_init(t_game *game)
{
	SDL_GameController	*controller;

	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
	{
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		cleanup(game);
	}
	controller = NULL;
	if (SDL_NumJoysticks() > 0 && SDL_IsGameController(0))
	{
		controller = SDL_GameControllerOpen(0);
		if (!controller)
		{
			printf("Failed to open controller: %s\n", SDL_GetError());
		}
		else
		{
			printf("Controller connected: %s\n", SDL_GameControllerName(controller));
		}
	}
	else
	{
		printf("No compatible controller found.\n");
	}
}

void	mouse_init(void)
{
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "0");
	SDL_SetRelativeMouseMode(SDL_TRUE);
}
