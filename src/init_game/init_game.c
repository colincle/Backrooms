/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	game_struct_init(t_game *game)
{
	init_maps(game);
	print_all_maps(game);
	init_vector_grid(game);
	init_entities(game);
	print_entities(game);
	KEYS = malloc(sizeof(int) * HOW_MANY_KEYS);
	if (!KEYS)
	{
		fprintf(stderr, "ERROR: memory allocation failed in game_struct_init");
		exit(EXIT_FAILURE);
	}
}

static void	graphics_init(t_game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		cleanup(game);
		exit(EXIT_FAILURE);
	}
	WINDOW = SDL_CreateWindow("SDLRaycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!WINDOW)
	{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		cleanup(game);
		exit(EXIT_FAILURE);
	}
	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!RENDERER)
	{
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		cleanup(game);
		exit(EXIT_FAILURE);
	}
	SDL_GetWindowSize(WINDOW, &WIND_WIDTH, &WIND_HEIGHT);
}

void	controller_init(t_game *game)
{
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) 
	{
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		SDL_Quit();
		cleanup(game);
		exit(EXIT_FAILURE);
	}
	SDL_GameController *controller = NULL;
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

t_game	*game_init(void)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	if (!game)
	{
		fprintf(stderr, "ERROR: memory allocation failed in game_init");
		exit(EXIT_FAILURE);
	}
	load_textures(game);
	controller_init(game);
	graphics_init(game);
	game_struct_init(game);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	return (game);
}
