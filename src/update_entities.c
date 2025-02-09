/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	move_player(t_game *game, int key)
{
	if (key == W)
	{
		PLAYER_X = PLAYER_X + (PLAYER_DIR_X * (PLAYER_SPEED) * (1.0 / FPS));
		PLAYER_Y = PLAYER_Y + (PLAYER_DIR_Y * (PLAYER_SPEED) * (1.0 / FPS));
	}
	if (key == S)
	{
		PLAYER_X = PLAYER_X - (PLAYER_DIR_X * (PLAYER_SPEED) * (1.0 / FPS));
		PLAYER_Y = PLAYER_Y - (PLAYER_DIR_Y * (PLAYER_SPEED) * (1.0 / FPS));
	}
}

static void	rotate_player(t_game *game, int key)
{
	double	old_dir_x;
	double	frame_time = 1.0 / game->fps;
	double	angle = ROTATION_SPEED * frame_time;
	double	sin_rot = sin(angle);
	double	cos_rot = cos(angle);

	old_dir_x = PLAYER_DIR_X;
	if (key == D)
	{
		PLAYER_DIR_X = (PLAYER_DIR_X * cos_rot - PLAYER_DIR_Y * sin_rot);
		PLAYER_DIR_Y = (old_dir_x * sin_rot + PLAYER_DIR_Y * cos_rot);
	}
	if (key == A)
	{
		PLAYER_DIR_X = (PLAYER_DIR_X * cos_rot + PLAYER_DIR_Y * sin_rot);
		PLAYER_DIR_Y = (-old_dir_x * sin_rot + PLAYER_DIR_Y * cos_rot);
	}
	set_player_cam(game, LEVEL);
}

static void	update_player(t_game *game)
{
	if (KEYS[W])
		move_player(game, W);
	if (KEYS[S])
		move_player(game, S);
	if (KEYS[A])
		rotate_player(game, A);
	if (KEYS[D])
		rotate_player(game, D);
}

t_dda_result	dda(float start_x, float start_y, float end_x, float end_y)
{
	t_dda_result	result;
	int				steps;
	float			dx;
	float			dy;
	float			x;
	float			y;
	float			x_inc;
	float			y_inc;
	int				i;

	dx = end_x - start_x;
	dy = end_y - start_y;
	steps = fabsf(dx) > fabsf(dy) ? fabsf(dx) : fabsf(dy);
	x_inc = dx / steps;
	y_inc = dy / steps;
	result.cells = malloc(sizeof(t_point) * (steps + 1));
	if (!result.cells)
	{
		result.count = 0;
		result.dir.x = 0;
		result.dir.y = 0;
		return (result);
	}
	x = start_x;
	y = start_y;
	result.count = steps + 1;
	i = 0;
	while (i < result.count)
	{
		result.cells[i].x = (int)floorf(x);
		result.cells[i].y = (int)floorf(y);
		x += x_inc;
		y += y_inc;
		i++;
	}
	float mag = sqrtf(dx * dx + dy * dy);
	if (mag == 0)
	{
		result.dir.x = 0;
		result.dir.y = 0;
	}
	else
	{
		result.dir.x = dx / mag;
		result.dir.y = dy / mag;
	}
	return (result);
}


static void	update_vector(t_game *game, int x, int y)
{
	t_dda_result	results;
	int		i;
	
	i = 0;
	results = dda(x + 0.5, y + 0.5, PLAYER_X, PLAYER_Y);
	while (results.count > i)
	{
		if (MAPS[LEVEL][results.cells[i].y][results.cells[i].x] != EMPTY)
			return ;
		i++;
	}
	VECTOR_GRID[LEVEL][y][x].x = results.dir.x;
	VECTOR_GRID[LEVEL][y][x].y = results.dir.y;
}

static void	update_vector_grid(t_game *game)
{
	int		x;
	int		y;
	
	y = 0;
	while (MAPS[LEVEL][y])
	{
		x = 0;
		while (MAPS[LEVEL][y][x])
		{
			if (MAPS[LEVEL][y][x] == EMPTY)
				update_vector(game, x, y);
			x++;
		}
		y++;
	}
}

void	update_entities(t_game *game)
{
	update_vector_grid(game);
	update_player(game);
}
