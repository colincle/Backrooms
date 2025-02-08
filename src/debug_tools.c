/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	show_fps(t_game *game)
{
	static int	i;
	static int	fps;

	if (!SHOW_FPS)
		return ;
	fps += FPS;
	i++;
	if (i == 100)
	{
		fps /= 100;
		printf("%d fps\n", fps);
		i = 0;
		fps = 0;
	}
}

void	draw_minimap(t_game *game)
{
	int	x;
	int	y;

	if (!SHOW_MINIMAP)
		return ;
	for (y = 0; MAPS[LEVEL][y] != NULL; y++)
	{
		for (x = 0; MAPS[LEVEL][y][x] != '\0'; x++)
		{
			SDL_Rect	cell = {x * MINIMAP_BLOCK_SIZE, y * MINIMAP_BLOCK_SIZE, MINIMAP_BLOCK_SIZE, MINIMAP_BLOCK_SIZE};
			if (MAPS[LEVEL][y][x] == EMPTY)
				SDL_SetRenderDrawColor(RENDERER, 255, 255, 0, 255);
			else if (MAPS[LEVEL][y][x] == WALL)
				SDL_SetRenderDrawColor(RENDERER, 128, 128, 128, 255);
			else if (MAPS[LEVEL][y][x] == DOOR_CLOSED)
				SDL_SetRenderDrawColor(RENDERER, 139, 69, 19, 255);
			SDL_RenderFillRect(RENDERER, &cell);
		}
	}
	SDL_SetRenderDrawColor(RENDERER, 255, 0, 0, 255);
	draw_cercle(RENDERER, PLAYER_X * MINIMAP_BLOCK_SIZE, PLAYER_Y * MINIMAP_BLOCK_SIZE, MINIMAP_BLOCK_SIZE / 4);
	SDL_RenderDrawLine(RENDERER, PLAYER_X * MINIMAP_BLOCK_SIZE, PLAYER_Y * MINIMAP_BLOCK_SIZE,
		PLAYER_X * MINIMAP_BLOCK_SIZE + (PLAYER_DIR_X * MINIMAP_BLOCK_SIZE),
		PLAYER_Y * MINIMAP_BLOCK_SIZE + (PLAYER_DIR_Y * MINIMAP_BLOCK_SIZE));
	SDL_RenderPresent(RENDERER);
}

void	print_entities(t_game *game)
{
	int	i;
	int	e;

	if (!PRINT_ENTITIES)
		return ;
	if (!game)
	{
		printf("Game structure is NULL.\n");
		return ;
	}
	printf("==== Entity Data ====\n");
	i = 0;
	while (i < NUMBER_OF_MAPS)
	{
		printf("Map %d:\n", i + 1);
		if (game->player && game->player[i])
			printf("  Player Start: X = %f, Y = %f, Dir = (%.1f, %.1f)\n",
				game->player[i]->x, game->player[i]->y,
				game->player[i]->dir.x, game->player[i]->dir.y);
		else
			printf("  No Player Found\n");
		if (game->enemy && game->enemy[i])
		{
			e = 0;
			while (game->enemy[i][e])
			{
				printf("  Enemy %d: X = %f, Y = %f, Dir = (%.1f, %.1f)\n",
					e + 1, game->enemy[i][e]->x, game->enemy[i][e]->y,
					game->enemy[i][e]->dir.x, game->enemy[i][e]->dir.y);
				e++;
			}
		}
		else
			printf("  No Enemies Found\n");
		printf("------------------------\n");
		i++;
	}
}

void	print_all_maps(t_game *game)
{
	int		i;
	int		j;

	i = 0;
	if (!PRINT_MAPS)
		return ;
	if (!game->maps)
	{
		printf("No maps loaded.\n");
		return ;
	}
	while (game->maps[i])
	{
		printf("Map %d:\n", i + 1);
		j = 0;
		while (game->maps[i][j])
		{
			printf("%s\n", game->maps[i][j]);
			j++;
		}
		printf("------------------------\n");
		i++;
	}
}
