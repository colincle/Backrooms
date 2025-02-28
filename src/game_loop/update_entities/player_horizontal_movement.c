/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	left_stick(t_game *game)
{
	static int	last_time;
	int			new_time;
	int			ellapsed;

	new_time = SDL_GetTicks();
	ellapsed = new_time - last_time;
	if (fabs(JOYSTICK_Y) < 0.1 && fabs(JOYSTICK_X) < 0.1)
	{
		if (ellapsed > 100)
		{
			if (!KEYS[SHIFT])
				PLAYER_SPEED = DEFAULT_SPEED;
		}
	}
	else
		last_time = new_time;
}

void	move_player(t_game *game, int key)
{
	float	x;
	float	y;

	if (key == W)
	{
		x = PLAYER_X + (PLAYER_DIR_X * PLAYER_SPEED * FRAME_TIME);
		y = PLAYER_Y + (PLAYER_DIR_Y * PLAYER_SPEED * FRAME_TIME);
	}
	if (key == S)
	{
		x = PLAYER_X - (PLAYER_DIR_X * PLAYER_SPEED * FRAME_TIME);
		y = PLAYER_Y - (PLAYER_DIR_Y * PLAYER_SPEED * FRAME_TIME);
	}
	if (key == A)
	{
		x = PLAYER_X + (PLAYER_DIR_Y * (PLAYER_SPEED * 0.6) * FRAME_TIME);
		y = PLAYER_Y - (PLAYER_DIR_X * (PLAYER_SPEED * 0.6) * FRAME_TIME);
	}
	if (key == D)
	{
		x = PLAYER_X - (PLAYER_DIR_Y * (PLAYER_SPEED * 0.6) * FRAME_TIME);
		y = PLAYER_Y + (PLAYER_DIR_X * (PLAYER_SPEED * 0.6) * FRAME_TIME);
	}
	collisions(game, x, y);
	if (PLAYER_SPEED == DEFAULT_SPEED)
		MOVING = WALKING;
	else
		MOVING = RUNNING;
}


void	move_player_joystick(t_game *game, float x, float y)
{
	float	move_x = PLAYER_X;
	float	move_y = PLAYER_Y;

	if (fabs(x) > 0.1)
	{
		move_x -= PLAYER_DIR_Y * (PLAYER_SPEED * 0.6) * (x * FRAME_TIME);
		move_y += PLAYER_DIR_X * (PLAYER_SPEED * 0.6) * (x * FRAME_TIME);
	}
	if (fabs(y) > 0.1)
	{
		move_x += PLAYER_DIR_X * PLAYER_SPEED * (y * FRAME_TIME);
		move_y += PLAYER_DIR_Y * PLAYER_SPEED * (y * FRAME_TIME);
	}
	collisions(game, move_x, move_y);
	if (PLAYER_SPEED == DEFAULT_SPEED)
		MOVING = WALKING;
	else
		MOVING = RUNNING;
}
