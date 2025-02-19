/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	keydown(t_game *game, SDL_KeyCode code)
{
	if (code == SDLK_w)
		KEYS[W] = TRUE;
	if (code == SDLK_a)
		KEYS[A] = TRUE;
	if (code == SDLK_s)
		KEYS[S] = TRUE;
	if (code == SDLK_d)
		KEYS[D] = TRUE;
	if (code == SDLK_LSHIFT)
	{
		KEYS[SHIFT] = TRUE;
		PLAYER_SPEED = DEFAULT_SPEED + 2;
	}
}

static void	keyup(t_game *game, SDL_KeyCode code)
{
	if (code == SDLK_w)
		KEYS[W] = FALSE;
	if (code == SDLK_a)
		KEYS[A] = FALSE;
	if (code == SDLK_s)
		KEYS[S] = FALSE;
	if (code == SDLK_d)
		KEYS[D] = FALSE;
	if (code == SDLK_LSHIFT)
	{
		KEYS[SHIFT] = FALSE;
		PLAYER_SPEED = DEFAULT_SPEED;
	}
}

void	handle_events(t_game *game, int *running)
{
	while (SDL_PollEvent(&EVENT))
	{
		printf("%d%c------------------------%c",EVENT.type, 10, 10); fflush(stdout); //debug
		if (EVENT.type == SDL_MOUSEMOTION)
		{
			MOUSE_X = EVENT.motion.xrel;
		}
		if (EVENT.type == SDL_CONTROLLERAXISMOTION)
		{
			if (EVENT.caxis.axis == 0)
				JOYSTICK_X = EVENT.caxis.value / 32768.0f;
			if (EVENT.caxis.axis == 1)
				JOYSTICK_Y = -EVENT.caxis.value / 32768.0f;
			if (EVENT.caxis.axis == 2)
				JOYSTICK_ROT = -EVENT.caxis.value / 32768.0f;
		}
		if (EVENT.type == SDL_KEYDOWN)
		{
			printf("%d%c------------------------%c", EVENT.key.keysym.sym, 10, 10); fflush(stdout); //debug
			keydown(game, EVENT.key.keysym.sym);
		}
		if (EVENT.type == SDL_KEYUP)
		{
			keyup(game, EVENT.key.keysym.sym);
		}
		if (EVENT.type == SDL_KEYDOWN && EVENT.key.keysym.sym == SDLK_ESCAPE)
		{
			*running = 0;
		}
		if (EVENT.type == SDL_CONTROLLERBUTTONDOWN && EVENT.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		{
			if (PLAYER_SPEED == DEFAULT_SPEED)
				PLAYER_SPEED = DEFAULT_SPEED + 2;
			else PLAYER_SPEED = DEFAULT_SPEED;
		}
		if (EVENT.type == SDL_QUIT)
		{
			*running = 0;
		}
		if (EVENT.type == SDL_WINDOWEVENT && EVENT.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			WIND_WIDTH = EVENT.window.data1;
			WIND_HEIGHT = EVENT.window.data2;
			if (WIND_WIDTH / WIND_HEIGHT != 1.7777)
			{
				WIND_HEIGHT = WIND_WIDTH * 9 / 16;
				SDL_SetWindowSize(WINDOW, WIND_WIDTH, WIND_HEIGHT);
			}
		}
	}
}
