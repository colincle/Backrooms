/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	crouch(t_game *game)
{
	int				eye_height_goal;
	static Uint32	press_start = 0;
	static int		processed = 0;
	Uint32			elapsed;

	if (JUMP != NO_JUMP)
		return ;
	if (KEYS[C])
	{
		if (press_start == 0)
		{
			press_start = SDL_GetTicks();
			processed = 0;
		}
		else
		{
			elapsed = SDL_GetTicks() - press_start;
			if (elapsed >= 150 && !processed)
			{
				CROUCH = CRAWLING;
				processed = 1;
			}
		}
	}
	else
	{
		if (press_start != 0)
		{
			elapsed = SDL_GetTicks() - press_start;
			if (elapsed < 150 && !processed)
			{
				if (CROUCH == STANDING)
					CROUCH = CROUCHING;
				else if (CROUCH == CROUCHING)
					CROUCH = STANDING;
			}
			else
			{
				if (CROUCH == CRAWLING)
					CROUCH = STANDING;
			}
		}
		press_start = 0;
		processed = 0;
	}
	if (CROUCH == STANDING)
		eye_height_goal = 0;
	if (CROUCH == CROUCHING)
		eye_height_goal = -250;
	if (CROUCH == CRAWLING)
		eye_height_goal = -620;
	if (EYE_HEIGHT < eye_height_goal)
	{
		EYE_HEIGHT += FPS * 0.5;
		if (EYE_HEIGHT > eye_height_goal)
			EYE_HEIGHT = eye_height_goal;
	}
	if (EYE_HEIGHT > eye_height_goal)
	{
		EYE_HEIGHT -= FPS * 0.5;
		if (EYE_HEIGHT < eye_height_goal)
			EYE_HEIGHT = eye_height_goal;
	}
}

void	jump(t_game *game)
{
	static float	jump_progress = 0.0f;
	static float	jump_origin = 0.0f;
	float			dt;
	float			eased;
	float			new_height;

	if (JUMP != JUMP_UP)
		return ;
	dt = 1.0f / game->fps;
	if (jump_progress == 0.0f)
		jump_origin = FEET_HEIGHT;
	jump_progress += dt * 3.0f;
	if (jump_progress >= 1.0f)
	{
		jump_progress = 1.0f;
		JUMP = NO_JUMP;
	}
	eased = 2 * jump_progress - jump_progress * jump_progress;
	new_height = jump_origin + eased * 500.0f;
	if (new_height >= 800.0f)
	{
		new_height = 800.0f;
		JUMP = NO_JUMP;
		jump_progress = 1.0f;
	}
	FEET_HEIGHT = new_height;
	if (!JUMP)
	{
		jump_progress = 0.0f;
		jump_origin = 0.0f;
	}
}

void	gravity(t_game *game)
{
	static float	vertical_speed = 0.0f;
	float			dt = 1.0f / FPS;
	int				goal_height;
	int				rise_speed = (int)(FPS * 0.5);

	if (JUMP)
		return ;
	goal_height = 0;
	if (STANDING_ON == EMPTY)
		goal_height = EMPTY_HEIGHT;
	else if (STANDING_ON == WALL_0)
		goal_height = WALL_0_HEIGHT;
	else if (STANDING_ON == WALL_1)
		goal_height = WALL_1_HEIGHT;
	else if (STANDING_ON == WALL_2)
		goal_height = WALL_1_HEIGHT + 100;
	else if (STANDING_ON == WALL_3)
		goal_height = WALL_1_HEIGHT + 200;
	if (FEET_HEIGHT > goal_height)
	{
		vertical_speed += GRAVITY * dt;
		FEET_HEIGHT -= (int)(vertical_speed);
		if (FEET_HEIGHT < goal_height)
		{
			FEET_HEIGHT = goal_height;
			vertical_speed = 0.0f;
		}
	}
	else if (FEET_HEIGHT < goal_height)
	{
		vertical_speed = 0.0f;
		rise_speed = (int)(FPS * 0.5);
		FEET_HEIGHT += rise_speed;
		if (FEET_HEIGHT > goal_height)
			FEET_HEIGHT = goal_height;
	}
}

void	update_player_height(t_game *game)
{
	PLAYER_HEIGHT = FEET_HEIGHT + EYE_HEIGHT;
}
