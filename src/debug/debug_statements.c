/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

#include <stdio.h>
#include <sys/time.h>

void	debug_statements(t_game *game)
{
	static struct timeval	last_time = {0, 0};
	static double			fps_sum = 0;
	static int				fps_count = 0;
	struct timeval			current_time;
	double					elapsed_time;

	gettimeofday(&current_time, NULL);
	elapsed_time = (current_time.tv_sec - last_time.tv_sec) +
					(current_time.tv_usec - last_time.tv_usec) / 1000000.0;
	
	fps_sum += FPS;
	fps_count++;

	if (elapsed_time >= 0.5)
	{
		if (SHOW_FPS)
			printf("Avg FPS (0.5s) = %f\n", fps_sum / fps_count);
		last_time = current_time;
		fps_sum = 0;
		fps_count = 0;
	}

	if (SHOW_DIRECTION)
		printf("dir x = %f dir y = %f\n", PLAYER_DIR_X, PLAYER_DIR_Y);
	if (SHOW_POSITION)
		printf("pos x = %f pos y = %f\n", PLAYER_X, PLAYER_Y);
	if (SHOW_CAM_PLANE)
		printf("camera plane x = %f camera plane y = %f\n", PLAYER_CAM_X, PLAYER_CAM_Y);
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
