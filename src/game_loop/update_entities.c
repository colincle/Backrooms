/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

Uint32 start_timer(Uint32 milliseconds)
{
	Uint32	timer_end;
	
	timer_end = SDL_GetTicks() + milliseconds;
	return (timer_end);
}

char *traversable(t_game *game, char c)
{
    // Blocks that are never traversable.
    if (strchr("#.D49", c))
        return NULL;

    // Blocks that are always traversable.
    if (strchr(" 0T", c))
        return (char *)"always";

    // Blocks that require the player to be short enough.
    if (strchr("5678", c))
    {
        if (PLAYER_HEIGHT < -540) {
            // All of 8,7,6,5 are OK.
            return (char *)"5-8";
        }
        else if (PLAYER_HEIGHT < -110) {
            // Only 7,6,5 are OK.
            if (strchr("765", c))
                return (char *)"5-7";
            else
                return NULL;
        }
        else if (PLAYER_HEIGHT < 320) {
            // Only 5 and 6 are OK.
            if (strchr("56", c))
                return (char *)"5-6";
            else
                return NULL;
        }
        else {
            return NULL;
        }
    }

    // Blocks that require the player to be tall enough.
    // Now walls '1', '2', and '3' all obey the same rule as wall '1'
    if (strchr("123", c))
    {
        if (FEET_HEIGHT >= WALL_1_HEIGHT)
            return (char *)"1-3";
        else
            return NULL;
    }

    return NULL;
}




#define COLLISION_RADIUS 0.3f


// Helper: returns the next lower block type in the ordering.
#include <stdlib.h>
#include <limits.h>

#define COLLISION_RADIUS 0.3f

// Helper: returns the next lower block type in the ordering.
char lower_block(char current) {
    switch (current) {
        case WALL_4: return WALL_3;
        case WALL_3: return WALL_2;
        case WALL_2: return WALL_1;
        case WALL_1: return WALL_0;
        case WALL_0: return EMPTY;
        default:     return current;  // Already EMPTY or unknown.
    }
}

// Helper: maps a block character to its height.
int get_block_height(char block) {
    if (block == EMPTY)
        return EMPTY_HEIGHT;
    else if (block == WALL_0)
        return WALL_0_HEIGHT;
    else if (block == WALL_1)
        return WALL_1_HEIGHT;
    else if (block == WALL_2)
        return WALL_2_HEIGHT;  // adjust as needed
    else if (block == WALL_3)
        return WALL_3_HEIGHT;  // adjust as needed
    else if (block == WALL_4)
        return WALL_4_HEIGHT;  // adjust as needed
    return 0;
}

// Returns TRUE if any diagonal point collides with a wall.
// It checks all four diagonal collision points and then updates STANDING_ON to the block
// (among those detected) whose height is closest to PLAYER_HEIGHT.
// If no collision point is found, it falls back to the block at the player's center.
int check_circle_collision(t_game *game, float x, float y)
{
    float diag_offset = COLLISION_RADIUS / sqrtf(2.0f);
    // Diagonal offsets: NE, NW, SE, SW.
    float offsets[4][2] = {
        {  diag_offset, -diag_offset },
        { -diag_offset, -diag_offset },
        {  diag_offset,  diag_offset },
        { -diag_offset,  diag_offset }
    };

    int collision_found = FALSE;
    int best_diff = INT_MAX;
    char best_block = STANDING_ON;
    int found = 0; // flag indicating if any collision block was detected

    for (int i = 0; i < 4; i++) {
        float px = x + offsets[i][0];
        float py = y + offsets[i][1];
        int cell_x = (int)floorf(px);
        int cell_y = (int)floorf(py);
        char block = MAPS[LEVEL][cell_y][cell_x];

        if (!traversable(game, block))
            collision_found = TRUE;

        int block_ht = get_block_height(block);
        int diff = abs(PLAYER_HEIGHT - block_ht);
        if (diff < best_diff) {
            best_diff = diff;
            best_block = block;
            found = 1;
        }

        // Debug output:
        fflush(stdout);
    }

    if (found)
        STANDING_ON = best_block;
    else
        STANDING_ON = MAPS[LEVEL][(int)floorf(y)][(int)floorf(x)];

    return collision_found;
}






void collisions(t_game *game, float new_x, float new_y)
{
    float old_x = PLAYER_X;
    float old_y = PLAYER_Y;
    float dx = new_x - old_x;
    float dy = new_y - old_y;

    // Try moving along X axis first
    float temp_x = old_x + dx;
    if (!check_circle_collision(game, temp_x, old_y))
        PLAYER_X = temp_x;
    // Then try moving along Y axis
    float temp_y = old_y + dy;
    if (!check_circle_collision(game, PLAYER_X, temp_y))
        PLAYER_Y = temp_y;
}




static void	move_player(t_game *game, int key)
{
	float	x;
	float	y;

	if (key == W)
	{
		x = PLAYER_X + (PLAYER_DIR_X * (PLAYER_SPEED) * (1.0 / FPS));
		y = PLAYER_Y + (PLAYER_DIR_Y * (PLAYER_SPEED) * (1.0 / FPS));
	}
	if (key == S)
	{
		x = PLAYER_X - (PLAYER_DIR_X * (PLAYER_SPEED) * (1.0 / FPS));
		y = PLAYER_Y - (PLAYER_DIR_Y * (PLAYER_SPEED) * (1.0 / FPS));
	}
	if (key == A)
	{
		x = PLAYER_X + (PLAYER_DIR_Y * (PLAYER_SPEED * 0.6) * (1.0 / FPS));
		y = PLAYER_Y - (PLAYER_DIR_X * (PLAYER_SPEED * 0.6) * (1.0 / FPS));
	}
	if (key == D)
	{
		x = PLAYER_X - (PLAYER_DIR_Y * (PLAYER_SPEED * 0.6) * (1.0 / FPS));
		y = PLAYER_Y + (PLAYER_DIR_X * (PLAYER_SPEED * 0.6) * (1.0 / FPS));
	}
	collisions(game, x, y);
	if (PLAYER_SPEED == DEFAULT_SPEED)
		MOVING = WALKING;
	else
		MOVING = RUNNING;
}

static void	move_player_joystick(t_game *game, float x, float y)
{
	float	move_x = PLAYER_X;
	float	move_y = PLAYER_Y;
	float	frame_time = 1.0 / FPS;

	if (fabs(x) > 0.1)
	{
		move_x -= PLAYER_DIR_Y * (PLAYER_SPEED * 0.6) * (x * frame_time);
		move_y += PLAYER_DIR_X * (PLAYER_SPEED * 0.6) * (x * frame_time);
	}
	if (fabs(y) > 0.1)
	{
		move_x += PLAYER_DIR_X * PLAYER_SPEED * (y * frame_time);
		move_y += PLAYER_DIR_Y * PLAYER_SPEED * (y * frame_time);
	}
	collisions(game, move_x, move_y);
	if (PLAYER_SPEED == DEFAULT_SPEED)
		MOVING = WALKING;
	else
		MOVING = RUNNING;
}

void	rotate_player_mouse(t_game *game, int x)
{
	double	old_dir_x;
	double	frame_time = 1.0 / game->fps;
	double	angle = abs(x) * MOUSE_SENSIT * frame_time;
	double	sin_rot = sin(angle);
	double	cos_rot = cos(angle);

	old_dir_x = PLAYER_DIR_X;
	if (x > 1)
	{
		PLAYER_DIR_X = (PLAYER_DIR_X * cos_rot - PLAYER_DIR_Y * sin_rot);
		PLAYER_DIR_Y = (old_dir_x * sin_rot + PLAYER_DIR_Y * cos_rot);
	}
	if (x < -1)
	{
		PLAYER_DIR_X = (PLAYER_DIR_X * cos_rot + PLAYER_DIR_Y * sin_rot);
		PLAYER_DIR_Y = (-old_dir_x * sin_rot + PLAYER_DIR_Y * cos_rot);
	}
	set_player_cam(game, LEVEL);
	MOUSE_X = 0;
}

void	rotate_player_joystick(t_game *game, float x)
{
	double	old_dir_x;
	double	frame_time = 1.0 / game->fps;
	double	angle = fabs(x) * JOY_SENSIT * frame_time;
	double	sin_rot = sin(angle);
	double	cos_rot = cos(angle);

	old_dir_x = PLAYER_DIR_X;
	if (x < -0.1)
	{
		PLAYER_DIR_X = (PLAYER_DIR_X * cos_rot - PLAYER_DIR_Y * sin_rot);
		PLAYER_DIR_Y = (old_dir_x * sin_rot + PLAYER_DIR_Y * cos_rot);
	}
	if (x > 0.1)
	{
		PLAYER_DIR_X = (PLAYER_DIR_X * cos_rot + PLAYER_DIR_Y * sin_rot);
		PLAYER_DIR_Y = (-old_dir_x * sin_rot + PLAYER_DIR_Y * cos_rot);
	}
	set_player_cam(game, LEVEL);
}

void	look_up_and_down_joystick(t_game *game, float x)
{
	double	frame_time = 1.0 / game->fps;
	double	offset = x * JOY_SENSIT * frame_time * 500;
	double	max_shift = WIND_HEIGHT * 0.15;

	CAM_SHIFT += offset;
	if (CAM_SHIFT > max_shift)
		CAM_SHIFT = max_shift;
	if (CAM_SHIFT < -max_shift)
		CAM_SHIFT = -max_shift;
}

void	look_up_and_down_mouse(t_game *game, int y)
{
	double	frame_time = 1.0 / game->fps;
	double	offset = abs(y) * MOUSE_SENSIT * frame_time * 500;
	double	max_shift = WIND_HEIGHT * 0.15;

	if (y > 1)
		CAM_SHIFT -= offset;
	if (y < -1)
		CAM_SHIFT += offset;
	if (CAM_SHIFT > max_shift)
		CAM_SHIFT = max_shift;
	if (CAM_SHIFT < -max_shift)
		CAM_SHIFT = -max_shift;
	set_player_cam(game, LEVEL);
	MOUSE_Y = 0;
}

void crouch(t_game *game)
{
	if (JUMP != NO_JUMP)
		return;

	// --- Input Processing (unchanged) ---
	static Uint32 press_start = 0;
	static int processed = 0; // 0 = not processed, 1 = processed

	if (KEYS[C]) {
		if (press_start == 0) {
			press_start = SDL_GetTicks();
			processed = 0;
		} else {
			Uint32 elapsed = SDL_GetTicks() - press_start;
			if (elapsed >= 150 && !processed) {
				// Held long enough: switch to crawling.
				CROUCH = CRAWLING;
				processed = 1;
			}
		}
	} else {
		if (press_start != 0) {
			Uint32 elapsed = SDL_GetTicks() - press_start;
			if (elapsed < 150 && !processed) {
				// Tap: toggle between standing and crouching.
				if (CROUCH == STANDING)
					CROUCH = CROUCHING;
				else if (CROUCH == CROUCHING)
					CROUCH = STANDING;
			} else {
				if (CROUCH == CRAWLING)
					CROUCH = STANDING;
			}
		}
		press_start = 0;
		processed = 0;
	}
	int	eye_height_goal;

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
void jump(t_game *game)
{
    if (JUMP != JUMP_UP)
        return;  // Only modify height during the upward phase.

    float dt = 1.0f / game->fps;  // Seconds per frame.
    const float jump_rate = 3.0f;   // Rate at which jump_progress changes.
    const float jump_range = 500.0f; // Fixed jump height offset from takeoff.
    const float head_cap = 800.0f;   // Maximum allowed height for the head.
    static float jump_progress = 0.0f; // 0 = on ground, 1 = apex.
    static float jump_origin = 0.0f;   // FEET_HEIGHT at jump takeoff.

    // Record takeoff height on the first frame.
    if (jump_progress == 0.0f)
        jump_origin = FEET_HEIGHT;

    jump_progress += dt * jump_rate;
    if (jump_progress >= 1.0f) {
        jump_progress = 1.0f;
        JUMP = NO_JUMP;
    }

    // Use quadratic easing: f(t) = 2*t - t^2, which yields f(1)=1 and f'(1)=0.
    float eased = 2 * jump_progress - jump_progress * jump_progress;
    // Now compute the new height from the fixed jump_origin.
    float new_height = jump_origin + eased * jump_range;

    // Clamp to head cap.
    if (new_height >= head_cap) {
        new_height = head_cap;
        JUMP = NO_JUMP;
        jump_progress = 1.0f;
    }

    FEET_HEIGHT = new_height;

    // Reset static variables after jump ends.
    if (!JUMP) {
        jump_progress = 0.0f;
        jump_origin = 0.0f;
    }
}






#define GRAVITY 100  // Gravity constant, adjust as needed.

void gravity(t_game *game)
{
	if (JUMP)
		return ;
	static float vertical_speed = 0.0f;
    float dt = 1.0f / FPS;  // Time per frame.
	int	goal_height;

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

    if (FEET_HEIGHT > goal_height) {
        vertical_speed += GRAVITY * dt;
        FEET_HEIGHT -= (int)(vertical_speed);
        if (FEET_HEIGHT < goal_height) {
            FEET_HEIGHT = goal_height;
            vertical_speed = 0.0f;
        }
    } else if (FEET_HEIGHT < goal_height) {
        vertical_speed = 0.0f;
        int rise_speed = (int)(FPS * 0.5);
        FEET_HEIGHT += rise_speed;
        if (FEET_HEIGHT > goal_height)
		FEET_HEIGHT = goal_height;
    }

}

void	update_player_height(t_game *game)
{
	PLAYER_HEIGHT = FEET_HEIGHT + EYE_HEIGHT;
}

static void	update_player(t_game *game)
{
	if (KEYS[W])
		move_player(game, W);
	if (KEYS[S])
		move_player(game, S);
	if (KEYS[A])
		move_player(game, A);
	if (KEYS[D])
		move_player(game, D);
	if (JOYSTICK_Y || JOYSTICK_X)
		move_player_joystick(game, JOYSTICK_X, JOYSTICK_Y);
	if (MOUSE_X)
		rotate_player_mouse(game, MOUSE_X);
	if (MOUSE_Y)
		look_up_and_down_mouse(game, MOUSE_Y);
	if (JOYSTICK_ROT_X)
		rotate_player_joystick(game, JOYSTICK_ROT_X);
	if (fabs(JOYSTICK_ROT_Y) > 0.1)
		look_up_and_down_joystick(game, JOYSTICK_ROT_Y);
	if (fabs(JOYSTICK_Y) < 0.1 && fabs(JOYSTICK_X) < 0.1 && !KEYS[W] && !KEYS[S] && !KEYS[A] && !KEYS[D])
		MOVING = STILL;
	gravity(game);
	if (JUMP)
		jump(game);
	crouch(game);
	update_player_height(game);

}

void	update_entities(t_game *game)
{
	update_vector_grid(game);
	update_player(game);
}
