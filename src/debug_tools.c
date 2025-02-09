/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	debug_statements(t_game *game)
{
	if (SHOW_FPS)
		printf("FPS = %f\n", FPS);
	if (SHOW_DIRECTION)
		printf("dir x = %f dir y = %f\n", PLAYER_DIR_X, PLAYER_DIR_Y);
	if (SHOW_POSITION)
		printf("pos x = %f pos y = %f\n", PLAYER_X, PLAYER_Y);
	if (SHOW_CAM_PLANE)
		printf("camera plane x = %f camera plane y = %f\n", PLAYER_CAM_X, PLAYER_CAM_Y);
}

static void	draw_vectors(t_game *game)
{
	int		x;
	int		y;
	t_float_xy	vec;
	SDL_Rect	center;

	for (y = 0; game->maps[LEVEL][y] != NULL; y++)
	{
		for (x = 0; game->maps[LEVEL][y][x] != '\0'; x++)
		{
			if (game->maps[LEVEL][y][x] != EMPTY)
				continue;
			vec = game->vector_grid[LEVEL][y][x];
			center.x = x * MINIMAP_BLOCK + MINIMAP_BLOCK / 2;
			center.y = y * MINIMAP_BLOCK + MINIMAP_BLOCK / 2;
			center.w = 2;
			center.h = 2;
			SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
			SDL_RenderFillRect(RENDERER, &center);
			if (vec.x == 0 && vec.y == 0)
			{
				SDL_RenderDrawLine(RENDERER, center.x - 3, center.y, center.x + 3, center.y);
				SDL_RenderDrawLine(RENDERER, center.x, center.y - 3, center.x, center.y + 3);
			}
			else
			{
				SDL_RenderDrawLine(RENDERER, center.x, center.y,
					center.x + vec.x * MINIMAP_BLOCK / 2,
					center.y + vec.y * MINIMAP_BLOCK / 2);
			}
		}
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
			SDL_Rect	cell = {x * MINIMAP_BLOCK, y * MINIMAP_BLOCK, MINIMAP_BLOCK, MINIMAP_BLOCK};
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
	draw_cercle(RENDERER, PLAYER_X * MINIMAP_BLOCK, PLAYER_Y * MINIMAP_BLOCK, MINIMAP_BLOCK / 4);
	SDL_RenderDrawLine(RENDERER, PLAYER_X * MINIMAP_BLOCK, PLAYER_Y * MINIMAP_BLOCK,
		PLAYER_X * MINIMAP_BLOCK + (PLAYER_DIR_X * MINIMAP_BLOCK),
		PLAYER_Y * MINIMAP_BLOCK + (PLAYER_DIR_Y * MINIMAP_BLOCK));
	draw_vectors(game);
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
			printf("Player Start: X = %f, Y = %f, Dir = (%.1f, %.1f)\n",
				game->player[i]->x, game->player[i]->y,
				game->player[i]->dir.x, game->player[i]->dir.y);
		else
			printf("  No Player Found\n");
		if (game->enemy && game->enemy[i])
		{
			e = 0;
			while (game->enemy[i][e])
			{
				printf("Enemy %d: X = %f, Y = %f, Dir = (%.1f, %.1f)\n",
					e + 1, game->enemy[i][e]->x, game->enemy[i][e]->y,
					game->enemy[i][e]->dir.x, game->enemy[i][e]->dir.y);
				e++;
			}
		}
		else
			printf("No Enemies Found\n");
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
