/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static char	*traversable(t_game *game, char c)
{
	if (strchr("#.D49", c))
		return (NULL);
	if (strchr(" 0T", c))
		return ((char *)"always");
	if (strchr("5678", c))
	{
		if (PLAYER_HEIGHT < -540)
		{
			return ((char *)"5-8");
		}
		else if (PLAYER_HEIGHT < -110)
		{
			if (strchr("765", c))
				return ((char *)"5-7");
			else
				return (NULL);
		}
		else if (PLAYER_HEIGHT < 320)
		{
			if (strchr("56", c))
				return ((char *)"5-6");
			else
				return (NULL);
		}
		else
		{
			return (NULL);
		}
	}
	if (strchr("123", c))
	{
		if (FEET_HEIGHT >= WALL_1_HEIGHT)
			return ((char *)"1-3");
		else
			return (NULL);
	}
	return (NULL);
}

static int	get_block_height(char block)
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
		block_ht = get_block_height(block);
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
