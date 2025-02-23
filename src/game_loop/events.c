/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	handle_events(t_game *game, int *running)
{
	while (SDL_PollEvent(&EVENT))
	{
		if (EVENT.type == SDL_MOUSEMOTION)
		{
			MOUSE_X = EVENT.motion.xrel;
			MOUSE_Y = EVENT.motion.yrel;
		}
		if (EVENT.type == SDL_CONTROLLERAXISMOTION)
		{
			if (EVENT.caxis.axis == 0)
				JOYSTICK_X = EVENT.caxis.value / 32768.0f;
			if (EVENT.caxis.axis == 1)
				JOYSTICK_Y = -EVENT.caxis.value / 32768.0f;
			if (EVENT.caxis.axis == 2)
				JOYSTICK_ROT_X = -EVENT.caxis.value / 32768.0f;
			if (EVENT.caxis.axis == 3)
				JOYSTICK_ROT_Y = -(EVENT.caxis.value / 32767.0f);
		}
		if (EVENT.type == SDL_KEYDOWN)
		{
			keydown(game, EVENT.key.keysym.sym);
		}
		if (EVENT.type == SDL_KEYUP)
		{
			keyup(game, EVENT.key.keysym.sym);
		}
		if (EVENT.type == SDL_KEYDOWN && EVENT.key.keysym.sym == SDLK_SPACE && !JUMP)
		{
			JUMP = JUMP_UP;
		}
		if (EVENT.type == SDL_KEYDOWN && EVENT.key.keysym.sym == SDLK_k && !JUMP)// to be removed
		{
			PLAYER_HEIGHT--;
			printf("PLAYER_HEIGHT = %d%c------------------------%c", PLAYER_HEIGHT, 10, 10); fflush(stdout); //debug
		}
		if (EVENT.type == SDL_KEYDOWN && EVENT.key.keysym.sym == SDLK_i && !JUMP)
		{
			PLAYER_HEIGHT++;
			printf("PLAYER_HEIGHT = %d%c------------------------%c", PLAYER_HEIGHT, 10, 10); fflush(stdout); //debug
		}
		if (EVENT.type == SDL_KEYDOWN && EVENT.key.keysym.sym == SDLK_l && !JUMP)// to be removed
		{
			PLAYER_HEIGHT = PLAYER_HEIGHT - 10;
			printf("PLAYER_HEIGHT = %d%c------------------------%c", PLAYER_HEIGHT, 10, 10); fflush(stdout); //debug
		}
		if (EVENT.type == SDL_KEYDOWN && EVENT.key.keysym.sym == SDLK_o && !JUMP)
		{
			PLAYER_HEIGHT = PLAYER_HEIGHT + 10;
			printf("PLAYER_HEIGHT = %d%c------------------------%c", PLAYER_HEIGHT, 10, 10); fflush(stdout); //debug
		}
		if (EVENT.type == SDL_CONTROLLERBUTTONDOWN && EVENT.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
		{
			if (PLAYER_SPEED == DEFAULT_SPEED)
				PLAYER_SPEED = DEFAULT_SPEED + 2;
			else
				PLAYER_SPEED = DEFAULT_SPEED;
		}
		if (EVENT.type == SDL_KEYDOWN && EVENT.key.keysym.sym == SDLK_ESCAPE)
		{
			*running = 0;
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
