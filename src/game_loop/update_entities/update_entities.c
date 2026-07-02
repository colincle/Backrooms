#include <SDLRaycaster.h>

static void	update_player(t_game *game)
{
	left_stick(game);
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
	crouch(game);
	gravity(game);
	update_player_height(game);
}

void	update_entities(t_game *game)
{
	update_player(game);
}
