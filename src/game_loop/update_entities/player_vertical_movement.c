#include <SDLRaycaster.h>

void	crouch(t_game *game)
{
	double			transition_speed;
	int				eye_height_goal = 0;
	static Uint32	press_start = 0;
	Uint32			elapsed;

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
	if (CROUCH_LOCK || FALLING)
		return ;
	eye_height_goal = 0;
	if (CROUCH == STANDING)
		eye_height_goal = TEXTURE_HEIGHT;
	else if (CROUCH == CROUCHING)
		eye_height_goal = CROUCH_HEIGHT;
	else if (CROUCH == CRAWLING)
		eye_height_goal = CRAWL_HEIGHT;
	transition_speed = 2000.0 * FRAME_TIME;
	if (EYE_HEIGHT < eye_height_goal && !STAND_LOCK)
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
	float			dt;
	float			eased;
	float			new_height;

	if (JUMP_LOCK || FALLING)
		JUMP = NO_JUMP;
	if (JUMP != JUMP_UP)
		return ;
	dt = FRAME_TIME;
	if (jump_progress == 0.0f)
		jump_origin = FEET_HEIGHT;
	jump_progress += dt * 3.0f;
	if (jump_progress >= 1.0f)
	{
		jump_progress = 1.0f;
		JUMP = NO_JUMP;
	}
	eased = 2 * jump_progress - jump_progress * jump_progress;
	new_height = jump_origin + eased * JUMP_HEIGHT;
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

void	respawn(t_game *game)
{
	FEET_HEIGHT = EMPTY_HEIGHT;
	FEET_TOUCH = EMPTY;
	FALLING = FALSE;
	PLAYER_X = game->player[LEVEL]->spawn.x;
	PLAYER_Y = game->player[LEVEL]->spawn.y;
	PLAYER_DIR_X = game->player[LEVEL]->spawn_dir.x;
	PLAYER_DIR_Y = game->player[LEVEL]->spawn_dir.y;
	set_player_cam(game, LEVEL);
}

void	gravity(t_game *game)
{
	static float	vertical_speed = 0.0f;
	float			dt = FRAME_TIME;
	int				goal_height;
	float			rise_speed = JUMP_HEIGHT * dt;

	if (JUMP)
		return ;
	goal_height = 0;
	if (FEET_TOUCH == HOLE)
		goal_height = HOLE_HEIGHT * 2;
	if (FEET_TOUCH == PILLAR)
		goal_height = EMPTY_HEIGHT;
	if (FEET_TOUCH == EMPTY)
		goal_height = EMPTY_HEIGHT;
	else if (FEET_TOUCH == WALL_0)
		goal_height = WALL_0_HEIGHT;
	else if (FEET_TOUCH == WALL_1)
		goal_height = WALL_1_HEIGHT;
	else if (FEET_TOUCH == WALL_2)
		goal_height = WALL_1_HEIGHT * 1.1;
	else if (FEET_TOUCH == WALL_3)
		goal_height = WALL_1_HEIGHT * 1.2;
	if (FEET_HEIGHT > goal_height)
	{
		FALLING = TRUE;
		vertical_speed += GRAVITY * dt;
		FEET_HEIGHT -= vertical_speed;
		if (FEET_HEIGHT <= goal_height)
		{
			FALLING = FALSE;
			FEET_HEIGHT = goal_height;
			vertical_speed = 0.0f;
		}
	}
	else if (FEET_HEIGHT < goal_height)
	{
		vertical_speed = 0.0f;
		FEET_HEIGHT += rise_speed * 2;
		if (FEET_HEIGHT > goal_height)
			FEET_HEIGHT = goal_height;
	}
	else if (FEET_HEIGHT == goal_height)
		FALLING = FALSE;
	if (FEET_HEIGHT <= HOLE_HEIGHT * 2)
	{
		respawn(game);
	}
}

void	update_player_height(t_game *game)
{
	PLAYER_HEIGHT = FEET_HEIGHT + EYE_HEIGHT;
}
