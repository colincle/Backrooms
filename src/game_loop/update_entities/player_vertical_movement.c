/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	crouch(t_game *game)
{
	int				eye_height_goal = 0; // Ensure it's always initialized
	static Uint32	press_start = 0;
	Uint32			elapsed;

	if (JUMP != NO_JUMP)
		return;

	if (KEYS[C])
	{
		if (press_start == 0)
			press_start = SDL_GetTicks();
		else
		{
			elapsed = SDL_GetTicks() - press_start;
			if (elapsed >= 150)
				CROUCH = CRAWLING;
		}
	}
	else
	{
		if (press_start != 0)
		{
			elapsed = SDL_GetTicks() - press_start;
			if (elapsed < 150)
			{
				if (CROUCH == STANDING)
					CROUCH = CROUCHING;
				else if (CROUCH == CROUCHING)
					CROUCH = STANDING;
			}
			else
			{
				CROUCH = STANDING;
			}
		}
		press_start = 0;
	}

	eye_height_goal = 0;
	if (CROUCH == STANDING)
		eye_height_goal = TEXTURE_HEIGHT;
	else if (CROUCH == CROUCHING)
		eye_height_goal = CROUCH_HEIGHT;
	else if (CROUCH == CRAWLING)
		eye_height_goal = CRAWL_HEIGHT;

	// Smooth eye height adjustment using FRAME_TIME
	double transition_speed = 2000.0 * FRAME_TIME;

	if (EYE_HEIGHT < eye_height_goal)
	{
		EYE_HEIGHT += transition_speed;
		if (EYE_HEIGHT > eye_height_goal)
			EYE_HEIGHT = eye_height_goal;
	}
	else if (EYE_HEIGHT > eye_height_goal)
	{
		EYE_HEIGHT -= transition_speed;
		if (EYE_HEIGHT < eye_height_goal)
			EYE_HEIGHT = eye_height_goal;
	}
}


void	jump(t_game *game)
{
	static float	jump_progress = 0.0f;
	static float	jump_origin = 0.0f;
	float		dt;
	float		eased;
	float		new_height;

	if (JUMP != JUMP_UP)
		return;
	dt = FRAME_TIME; // Use actual frame time

	if (jump_progress == 0.0f)
		jump_origin = FEET_HEIGHT;

	jump_progress += dt * 3.0f; // Ensure progress scales with time
	if (jump_progress >= 1.0f)
	{
		jump_progress = 1.0f;
		JUMP = NO_JUMP;
	}

	eased = 2 * jump_progress - jump_progress * jump_progress;
	new_height = jump_origin + eased * JUMP_HEIGHT;

	// Fix: HEIGHT_CAP is already set correctly, no need for set_height()
	if (new_height >= HEIGHT_CAP)
	{
		new_height = HEIGHT_CAP;
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
	float		dt = FRAME_TIME; // Use actual frame time
	int			goal_height;
	float		rise_speed = JUMP_HEIGHT * dt; // Smooth rising speed

	if (JUMP)
		return;
	goal_height = 0;
	if (STANDING_ON == EMPTY)
		goal_height = EMPTY_HEIGHT;
	else if (STANDING_ON == WALL_0)
		goal_height = WALL_0_HEIGHT;
	else if (STANDING_ON == WALL_1)
		goal_height = WALL_1_HEIGHT;
	else if (STANDING_ON == WALL_2)
		goal_height = WALL_1_HEIGHT * 1.1;
	else if (STANDING_ON == WALL_3)
		goal_height = WALL_1_HEIGHT  * 1.2;
	// Apply gravity if falling
	if (FEET_HEIGHT > goal_height)
	{
		vertical_speed += GRAVITY * dt; // Gravity scales properly with time
		FEET_HEIGHT -= vertical_speed;
		if (FEET_HEIGHT < goal_height)
		{
			FEET_HEIGHT = goal_height;
			vertical_speed = 0.0f;
		}
	}
	// Apply smooth rising movement
	else if (FEET_HEIGHT < goal_height)
	{
		vertical_speed = 0.0f;
		FEET_HEIGHT += rise_speed * 2;
		if (FEET_HEIGHT > goal_height)
			FEET_HEIGHT = goal_height;
	}
}


void	update_player_height(t_game *game)
{
	PLAYER_HEIGHT = FEET_HEIGHT + EYE_HEIGHT;
}
