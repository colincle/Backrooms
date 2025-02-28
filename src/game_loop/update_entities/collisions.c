/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static int	traversable(t_game *game, char c)
{
	if (c == WALL || c == WALL_4 || c == WALL_9)
		return (0);
	else if (c == WALL_1)
		return (FEET_HEIGHT >= WALL_1_HEIGHT);
	else if (c == WALL_2)
		return (FEET_HEIGHT >= WALL_1_HEIGHT);
	else if (c == WALL_3)
		return (FEET_HEIGHT >= WALL_1_HEIGHT);
	else if (c == WALL_5)
		return (PLAYER_HEIGHT < WALL_5_HEIGHT);
	else if (c == WALL_6)
		return (PLAYER_HEIGHT < WALL_6_HEIGHT);
	else if (c == WALL_7)
		return (PLAYER_HEIGHT < WALL_7_HEIGHT);
	else if (c == WALL_8)
		return (PLAYER_HEIGHT < WALL_8_HEIGHT);
	return (1);
}

static int	get_block_height(t_game *game, char block)
{
	if (block == EMPTY)
		return (EMPTY_HEIGHT);
	else if (block == WALL_0)
		return (WALL_0_HEIGHT);
	else if (block == WALL_1)
		return (WALL_1_HEIGHT);
	else if (block == WALL_2)
		return (WALL_2_HEIGHT);
	else if (block == WALL_3)
		return (WALL_3_HEIGHT);
	else if (block == WALL_4)
		return (WALL_4_HEIGHT);
	return (0);
}

static int	check_circle_collision(t_game *game, float x, float y)
{
	int		i, cell_x, cell_y, block_ht, diff, collision_found;
	int		best_diff, found;
	char	block, best_block;
	float	px, py, diag_offset, offsets[4][2];

	collision_found = FALSE;
	best_diff = INT_MAX;
	best_block = STANDING_ON;
	found = 0;
	diag_offset = COLLISION_RADIUS / sqrtf(2.0f);
	offsets[0][0] = diag_offset;
	offsets[0][1] = -diag_offset;
	offsets[1][0] = -diag_offset;
	offsets[1][1] = -diag_offset;
	offsets[2][0] = diag_offset;
	offsets[2][1] = diag_offset;
	offsets[3][0] = -diag_offset;
	offsets[3][1] = diag_offset;
	i = -1;
	while (++i < 4)
	{
		px = x + offsets[i][0];
		py = y + offsets[i][1];
		cell_x = (int)floorf(px);
		cell_y = (int)floorf(py);
		block = MAPS[LEVEL][cell_y][cell_x];
		if (!traversable(game, block))
			collision_found = TRUE;
		block_ht = get_block_height(game, block);
		diff = abs(PLAYER_HEIGHT - block_ht);
		if (diff < best_diff)
		{
			best_diff = diff;
			best_block = block;
			found = 1;
		}
	}
	STANDING_ON = found ? best_block : MAPS[LEVEL][(int)floorf(y)][(int)floorf(x)];
	return (collision_found);
}

void	collisions(t_game *game, float new_x, float new_y)
{
	float	old_x = PLAYER_X;
	float	old_y = PLAYER_Y;
	float	dx = new_x - old_x;
	float	dy = new_y - old_y;
	float	temp_y = old_y + dy;
	float	temp_x = old_x + dx;

	if (!check_circle_collision(game, temp_x, old_y))
		PLAYER_X = temp_x;
	if (!check_circle_collision(game, PLAYER_X, temp_y))
		PLAYER_Y = temp_y;
}
