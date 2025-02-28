/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	controller_keydown(t_game *game, SDL_GameControllerButton button)
{
	if (button == SDL_CONTROLLER_BUTTON_A)
		JUMP = JUMP_UP;
	else if (button == SDL_CONTROLLER_BUTTON_B)
		KEYS[C] = TRUE;
	else if (SDL_CONTROLLER_BUTTON_LEFTSTICK)
		PLAYER_SPEED = (PLAYER_SPEED == DEFAULT_SPEED) ? DEFAULT_SPEED + 2 : DEFAULT_SPEED;
}

void	controller_keyup(t_game *game, SDL_GameControllerButton button)
{
	if (button == SDL_CONTROLLER_BUTTON_B)
		KEYS[C] = FALSE;
}
