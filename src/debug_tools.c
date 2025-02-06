/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	print_entities(t_game *game)
{
	int	i;
	int	e;
	if (!PRINT_ENTITIES)
		return ;
	if (!game)
	{
		printf("Game structure is NULL.\n");
		return;
	}
	printf("==== Entity Data ====\n");
	i = 0;
	while (i < NUMBER_OF_MAPS)
	{
		printf("Map %d:\n", i + 1);
		if (game->player_start && game->player_start[i])
			printf("  Player Start: X = %d, Y = %d, Dir = (%.1f, %.1f)\n",
				game->player_start[i]->x, game->player_start[i]->y,
				game->player_start[i]->dir.x, game->player_start[i]->dir.y);
		else
			printf("  No Player Found\n");
		if (game->enemy_start && game->enemy_start[i])
		{
			e = 0;
			while (game->enemy_start[i][e])
			{
				printf("  Enemy %d: X = %d, Y = %d, Dir = (%.1f, %.1f)\n",
					e + 1, game->enemy_start[i][e]->x, game->enemy_start[i][e]->y,
					game->enemy_start[i][e]->dir.x, game->enemy_start[i][e]->dir.y);
				e++;
			}
		}
		else
			printf("  No Enemies Found\n");
		printf("------------------------\n");
		i++;
	}
}

void print_all_maps(t_game *game)
{
	if (!PRINT_MAPS)
		return ;
	if (!game->maps)
	{
		printf("No maps loaded.\n");
		return;
	}

	int i = 0;
	while (game->maps[i])
	{
		printf("Map %d:\n", i + 1);

		int j = 0;
		while (game->maps[i][j])
		{
			printf("%s\n", game->maps[i][j]);
			j++;
		}

		printf("------------------------\n");
		i++;
	}
}