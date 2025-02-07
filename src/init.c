/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void find_entities(t_game *game, int i)
{
	int y = 0;
	int x;
	int e = 0;

	while (MAPS[i][y])
	{
		x = 0;
		while (MAPS[i][y][x])
		{
			if (MAPS[i][y][x] == P_NORTH || MAPS[i][y][x] == P_SOUTH ||
				MAPS[i][y][x] == P_WEST || MAPS[i][y][x] == P_EAST)
			{
				game->player[i] = malloc(sizeof(t_entity));
				game->player[i]->x = x + 0.5;
				game->player[i]->y = y + 0.5;
				if (MAPS[i][y][x] == P_NORTH)
				{
					game->player[i]->dir.x = 0; 
					game->player[i]->dir.y = -1; 
				}
				if (MAPS[i][y][x] == P_SOUTH)
				{
					game->player[i]->dir.x = 0; 
					game->player[i]->dir.y = 1; 
				}
				if (MAPS[i][y][x] == P_EAST)
				{
					game->player[i]->dir.x = 1; 
					game->player[i]->dir.y = 0; 
				}
				if (MAPS[i][y][x] == P_WEST)
				{
					game->player[i]->dir.x = -1; 
					game->player[i]->dir.y = 0; 
				}
				MAPS[i][y][x] = EMPTY;
			}
			if (MAPS[i][y][x] == E_NORTH || MAPS[i][y][x] == E_SOUTH ||
				MAPS[i][y][x] == E_WEST || MAPS[i][y][x] == E_EAST)
			{
				game->enemy[i] = realloc(game->enemy[i], sizeof(t_entity *) * (e + 1));
				game->enemy[i][e] = malloc(sizeof(t_entity));
				game->enemy[i][e]->x = x + 0.5;
				game->enemy[i][e]->y = y + 0.5;
				if (MAPS[i][y][x] == E_NORTH)
				{
					game->enemy[i][e]->dir.x = 0; 
					game->enemy[i][e]->dir.y = -1; 
				}
				if (MAPS[i][y][x] == E_SOUTH)
				{
					game->enemy[i][e]->dir.x = 0; 
					game->enemy[i][e]->dir.y = 1; 
				}
				if (MAPS[i][y][x] == E_EAST)
				{
					game->enemy[i][e]->dir.x = 1; 
					game->enemy[i][e]->dir.y = 0; 
				}
				if (MAPS[i][y][x] == E_WEST)
				{
					game->enemy[i][e]->dir.x = -1; 
					game->enemy[i][e]->dir.y = 0; 
				}
				e++;
				MAPS[i][y][x] = EMPTY;
			}
			x++;
		}
		y++;
	}
}

void init_entities(t_game *game)
{
	int i = 0;

	game->player = malloc(sizeof(t_entity *) * NUMBER_OF_MAPS);
	game->enemy = malloc(sizeof(t_entity **) * NUMBER_OF_MAPS);
	while (i < NUMBER_OF_MAPS)
	{
		game->enemy[i] = NULL;
		find_entities(game, i);
		i++;
	}
}

void	init_maps(t_game *game)
{
	char *path;
	int i = 1;

	MAPS = malloc(sizeof(char **) * (NUMBER_OF_MAPS + 1));
	if (!MAPS)
		return;
	while (i <= NUMBER_OF_MAPS)
	{
		path = get_path(i);
		MAPS[i - 1] = get_map(path);
		free(path);
		if (!MAPS[i - 1])
		{
			free(MAPS);
			MAPS = NULL;
			return;
		}
		i++;
	}
	MAPS[NUMBER_OF_MAPS] = NULL;
}

void	game_struct_init(t_game *game)
{
	game->frames = malloc(sizeof(t_frames));
	if (!game->frames)
	{
		cleanup(game);
		exit(EXIT_FAILURE);
	}
	init_maps(game);
	print_all_maps(game);
	init_entities(game);
	print_entities(game);
	KEYS = malloc(sizeof(int) *  HOW_MANY_KEYS);
}

void	graphics_init(t_game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		cleanup(game);
		exit(EXIT_FAILURE);
	}
	WINDOW = SDL_CreateWindow("SDLRaycaster", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
	if (!WINDOW)
	{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		cleanup(game);
		exit(EXIT_FAILURE);
	}
	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
	if (!RENDERER)
	{
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		cleanup(game);
		exit(EXIT_FAILURE);
	}
	SDL_GetWindowSize(WINDOW, &WIND_WIDTH, &WIND_HEIGHT);

}

t_game	*game_init()
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	if (!game)
		exit(EXIT_FAILURE);
	game_struct_init(game);
	graphics_init(game);
	return (game);
}
