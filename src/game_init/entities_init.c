#include <SDLRaycaster.h>

void	set_player_cam(t_game *game, int i)
{
	double	fov_rad;
	double	h_fov_rad;
	double	aspect_ratio = (double)TEXTURE_WIDTH / (double)TEXTURE_HEIGHT;

	fov_rad = (FOV * M_PI) / 180.0;
	h_fov_rad = 2 * atan(tan(fov_rad / 2) * aspect_ratio);
	PLAYER[i]->cam.x = -PLAYER[i]->dir.y * tan(h_fov_rad / 2);
	PLAYER[i]->cam.y = PLAYER[i]->dir.x * tan(h_fov_rad / 2);
}

static void	find_entities(t_game *game, int i)
{
	int	x;
	int	y = 0;
	int	e = 0;

	game->enemy[i] = NULL;
	while (MAPS[i][y])
	{
		x = 0;
		while (MAPS[i][y][x])
		{
			if (MAPS[i][y][x] == P_NORTH || MAPS[i][y][x] == P_SOUTH ||
				MAPS[i][y][x] == P_WEST || MAPS[i][y][x] == P_EAST)
			{
				game->player[i] = malloc(sizeof(t_player));
				if (!game->player[i])
				{
					fprintf(stderr, "ERROR: memory allocation failed in find_entities");
					exit(EXIT_FAILURE);
				}
				game->player[i]->camera_shift = 0;
				game->player[i]->feet_touch = EMPTY;
				game->player[i]->jumping = NO_JUMP;
				game->player[i]->crouching = 0;
				game->player[i]->stand_lock = 0;
				game->player[i]->crouch_lock = 0;
				game->player[i]->falling = 0;
				game->player[i]->jump_lock = 0;
				game->player[i]->speed = DEFAULT_SPEED;
				game->player[i]->x = x + 0.5;
				game->player[i]->y = y + 0.5;
				game->player[i]->spawn.x = x + 0.5;
				game->player[i]->spawn.y = y + 0.5;
				if (MAPS[i][y][x] == P_NORTH)
				{
					game->player[i]->dir.x = 0;
					game->player[i]->dir.y = -1;
					game->player[i]->spawn_dir.x = 0;
					game->player[i]->spawn_dir.y = -1;
				}
				if (MAPS[i][y][x] == P_SOUTH)
				{
					game->player[i]->dir.x = 0;
					game->player[i]->dir.y = 1;
					game->player[i]->spawn_dir.x = 0;
					game->player[i]->spawn_dir.y = 1;
				}
				if (MAPS[i][y][x] == P_EAST)
				{
					game->player[i]->dir.x = 1;
					game->player[i]->dir.y = 0;
					game->player[i]->spawn_dir.x = 1;
					game->player[i]->spawn_dir.y = 0;
				}
				if (MAPS[i][y][x] == P_WEST)
				{
					game->player[i]->dir.x = -1;
					game->player[i]->dir.y = 0;
					game->player[i]->spawn_dir.x = -1;
					game->player[i]->spawn_dir.y = 0;
				}
				MAPS[i][y][x] = EMPTY;
			}
			if (MAPS[i][y][x] == E_NORTH || MAPS[i][y][x] == E_SOUTH ||
				MAPS[i][y][x] == E_WEST || MAPS[i][y][x] == E_EAST)
			{
				game->enemy[i] = realloc(game->enemy[i], sizeof(t_player *) * (e + 2));
				if (!game->enemy[i])
				{
					fprintf(stderr, "ERROR: memory allocation failed in find_entities");
					exit(EXIT_FAILURE);
				}
				game->enemy[i][e] = malloc(sizeof(t_player));
				if (!game->enemy[i][e])
				{
					fprintf(stderr, "ERROR: memory allocation failed in find_entities");
					exit(EXIT_FAILURE);
				}
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
	game->enemy[i][e] = NULL;
}

void	entities_init(t_game *game)
{
	int	i = 0;

	game->player = malloc(sizeof(t_player *) * NUMBER_OF_MAPS);
	if (!game->player)
	{
		fprintf(stderr, "ERROR: memory allocation failed in entities_init");
		cleanup(game);
	}
	game->enemy = malloc(sizeof(t_enemy **) * NUMBER_OF_MAPS);
	if (!game->enemy)
	{
		fprintf(stderr, "ERROR: memory allocation failed in entities_init");
		cleanup(game);
	}
	while (i < NUMBER_OF_MAPS)
	{
		game->enemy[i] = NULL;
		find_entities(game, i);
		set_player_cam(game, i);
		i++;
	}
}
