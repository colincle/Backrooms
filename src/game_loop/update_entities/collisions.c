#include <SDLRaycaster.h>

static int	traversable(t_game *game, char c)
{
	if (c == WALL || c == WALL_4 || c == WALL_9)
		return (0);
	if (c == EMPTY || c == WALL_0)
		return (FEET_HEIGHT >= EMPTY_HEIGHT);
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
	if (block == HOLE)
		return (HOLE_HEIGHT);
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
	else if (block == WALL_5)
		return (WALL_5_HEIGHT);
	else if (block == WALL_6)
		return (WALL_6_HEIGHT);
	else if (block == WALL_7)
		return (WALL_7_HEIGHT);
	else if (block == WALL_8)
		return (WALL_8_HEIGHT);
	return (0);
}

static int	check_circle_collision(t_game *game, float x, float y)
{
	int		i, cell_x, cell_y, block_ht, diff, collision_found;
	int		best_diff, found;
	int		ceiling_best_diff;
	char	block, best_block;
	char	ceiling_best_block;
	float	px, py, diag_offset, offsets[4][2];

	collision_found = FALSE;
	best_diff = INT_MAX;
	ceiling_best_diff = -INT_MAX;
	best_block = FEET_TOUCH;
	ceiling_best_block = EMPTY;
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
		if (block == WALL_5 || block == WALL_6 || block == WALL_7 || block == WALL_8)
		{
			if (block_ht > PLAYER_HEIGHT && block_ht > ceiling_best_diff)
			{
				ceiling_best_diff = block_ht;
				ceiling_best_block = block;
			}
		}
		else
		{
			diff = abs(FEET_HEIGHT - block_ht);
			if (diff < best_diff)
			{
				best_diff = diff;
				best_block = block;
				found = 1;
			}
		}
	}
	HEAD_TOUCH = ceiling_best_block;
	if (found)
		FEET_TOUCH = best_block;
	else
		FEET_TOUCH = MAPS[LEVEL][(int)floorf(y)][(int)floorf(x)];
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
	if (HEAD_TOUCH == WALL_7 || HEAD_TOUCH == WALL_8)
	{
		JUMP_LOCK = TRUE;
		STAND_LOCK = TRUE;
	}
	else if (FEET_TOUCH == WALL_2 || FEET_TOUCH == WALL_3)
	{
		JUMP_LOCK = TRUE;
		CROUCH_LOCK = TRUE;
	}
	else
	{
		CROUCH_LOCK = FALSE;
		JUMP_LOCK = FALSE;
		STAND_LOCK = FALSE;
	}
	if (HEAD_TOUCH == WALL_5)
		HEIGHT_CAP = W5_HEIGHT_CAP;
	else if (HEAD_TOUCH == WALL_6)
		HEIGHT_CAP = W6_HEIGHT_CAP;
	else
		HEIGHT_CAP = EMPTY_HEIGHT_CAP;
}
