#include <SDLRaycaster.h>

void	keydown(t_game *game, SDL_KeyCode code)
{
	if (code == SDLK_w)
		KEYS[W] = TRUE;
	if (code == SDLK_a)
		KEYS[A] = TRUE;
	if (code == SDLK_s)
		KEYS[S] = TRUE;
	if (code == SDLK_d)
		KEYS[D] = TRUE;
	if (code == SDLK_c)
		KEYS[C] = TRUE;
	if (code == SDLK_LSHIFT)
	{
		KEYS[SHIFT] = TRUE;
		PLAYER_SPEED = DEFAULT_SPEED + 2;
	}
}

void	keyup(t_game *game, SDL_KeyCode code)
{
	if (code == SDLK_w)
		KEYS[W] = FALSE;
	if (code == SDLK_a)
		KEYS[A] = FALSE;
	if (code == SDLK_s)
		KEYS[S] = FALSE;
	if (code == SDLK_d)
		KEYS[D] = FALSE;
	if (code == SDLK_c)
		KEYS[C] = FALSE;
	if (code == SDLK_LSHIFT)
	{
		KEYS[SHIFT] = FALSE;
		PLAYER_SPEED = DEFAULT_SPEED;
	}
}
