/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

int	traversable(char c)
{
	return
	(
		c != WALL
		&& c != WALL
		&& c != WALL_1
		&& c != WALL_2
		&& c != WALL_3
		&& c != WALL_4
		&& c != WALL_7
		&& c != WALL_8
		&& c != WALL_9
		&& c != VOID
		&& c != DOOR_CLOSED
	);
}

#define COLLISION_RADIUS 0.3f

// Returns true if the circle centered at (x,y) with radius COLLISION_RADIUS collides with a wall.
int check_circle_collision(t_game *game, float x, float y)
{
    int cell_min_x = (int)floorf(x - COLLISION_RADIUS);
    int cell_max_x = (int)ceilf(x + COLLISION_RADIUS);
    int cell_min_y = (int)floorf(y - COLLISION_RADIUS);
    int cell_max_y = (int)ceilf(y + COLLISION_RADIUS);

    for (int j = cell_min_y; j <= cell_max_y; j++) {
        for (int i = cell_min_x; i <= cell_max_x; i++) {
            if (!traversable(MAPS[LEVEL][j][i])) {
                // Compute the nearest point on the cell (i,j) to (x,y)
                float nearest_x = fmaxf(i, fminf(x, i + 1));
                float nearest_y = fmaxf(j, fminf(y, j + 1));
                float dx = x - nearest_x;
                float dy = y - nearest_y;
                if ((dx * dx + dy * dy) < (COLLISION_RADIUS * COLLISION_RADIUS))
                    return TRUE;
            }
        }
    }
    return FALSE;
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

void jump(t_game *game)
{
    float dt = 1.0f / game->fps;  // time per frame in seconds
    const float jump_rate = 3.0f; // controls how fast the jump progresses (progress units per second)
    const float jump_range = 500.0f; // jump range from base height
    const float head_cap = 835.0f;   // maximum allowed height (player's head hits the ceiling)
    static float jump_progress = 0.0f; // 0 = on the ground, 1 = apex

    if (JUMP == JUMP_UP)
    {
        jump_progress += dt * jump_rate;
        if (jump_progress >= 1.0f)
        {
            jump_progress = 1.0f;
            JUMP = JUMP_DOWN;
        }
    }
    else if (JUMP == JUMP_DOWN)
    {
        jump_progress -= dt * jump_rate;
        if (jump_progress <= 0.0f)
        {
            jump_progress = 0.0f;
            JUMP = NO_JUMP;
        }
    }

    // Easing function (ease-out quadratic) for a natural jump curve:
    float eased = 2 * jump_progress - jump_progress * jump_progress;
    // Compute the new height from PLAYER_BASE_HEIGHT to PLAYER_BASE_HEIGHT + jump_range.
    float new_height = BASE_HEIGHT + jump_range * eased;
    // Cap the height so the player's head doesn't hit the ceiling.
    if (new_height > head_cap)
        new_height = head_cap;
    PLAYER_HEIGHT = new_height;
}

void	crawl(t_game *game)
{
	PLAYER_SPEED = DEFAULT_SPEED;
	if (PLAYER_HEIGHT > BASE_HEIGHT - 620)
	{
		PLAYER_HEIGHT -= 10;
		if (PLAYER_HEIGHT < BASE_HEIGHT - 620)
			PLAYER_HEIGHT = BASE_HEIGHT - 620;
	}
}

void crouch(t_game *game)
{
    PLAYER_SPEED = DEFAULT_SPEED;
    // If we're in a crouched state, lower the player gradually
    if (CROUCH != STANDING)
    {
        if (PLAYER_HEIGHT > BASE_HEIGHT - 200)
        {
            PLAYER_HEIGHT -= 10;
            if (PLAYER_HEIGHT < BASE_HEIGHT - 200)
                PLAYER_HEIGHT = BASE_HEIGHT - 200;
        }
    }
    // Otherwise, if not crouching, gradually return to standing height
    else
    {
        if (PLAYER_HEIGHT < BASE_HEIGHT)
        {
            PLAYER_HEIGHT += 10;
            if (PLAYER_HEIGHT > BASE_HEIGHT)
                PLAYER_HEIGHT = BASE_HEIGHT;
        }
    }
}

void crouch_toggle(t_game *game)
{
    static float hold_time = 0.0f;
    static int was_pressed = FALSE;
    float dt = 1.0f / game->fps;
    const float hold_threshold = 0.3f; // seconds to distinguish a tap from a hold

    int c_pressed = KEYS[C];

    if (c_pressed) {
        // Key is pressed; accumulate hold time.
        hold_time += dt;
        // If held beyond the threshold, force crawling.
        if (hold_time >= hold_threshold) {
            CROUCH = CRAWLING;
        }
    } else {
        // Key is released.
        if (was_pressed) { // A release event
            if (hold_time < hold_threshold) {
                // It was a tap: toggle between standing and crouching.
                if (CROUCH == STANDING)
                    CROUCH = CROUCHING;
                else if (CROUCH == CROUCHING)
                    CROUCH = STANDING;
            } else {
                // It was held: ensure we go to standing upon release.
                CROUCH = STANDING;
            }
        }
        // Reset hold time on release.
        hold_time = 0.0f;
    }

    was_pressed = c_pressed;
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
		if (JUMP)
		jump(game);
		crouch_toggle(game);
		if (CROUCH == CRAWLING)
			crawl(game);
		if (CROUCH == CROUCHING)
			crouch(game);
}

void	update_entities(t_game *game)
{
	update_vector_grid(game);
	update_player(game);
}
