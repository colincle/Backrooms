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
		else if (EVENT.type == SDL_CONTROLLERAXISMOTION)
		{
			if (EVENT.caxis.axis == 0)
				JOYSTICK_X = EVENT.caxis.value / 32768.0f;
			else if (EVENT.caxis.axis == 1)
				JOYSTICK_Y = -EVENT.caxis.value / 32768.0f;
			else if (EVENT.caxis.axis == 2)
				JOYSTICK_ROT_X = -EVENT.caxis.value / 32768.0f;
			else if (EVENT.caxis.axis == 3)
				JOYSTICK_ROT_Y = -(EVENT.caxis.value / 32767.0f);
		}
		else if (EVENT.type == SDL_KEYDOWN)
		{
			keydown(game, EVENT.key.keysym.sym);
			if (EVENT.key.keysym.sym == SDLK_SPACE)
			{
				CROUCH = STANDING;
				JUMP = JUMP_UP;
			}
#ifndef __EMSCRIPTEN__
			// in the browser, escape only releases the mouse
			else if (EVENT.key.keysym.sym == SDLK_ESCAPE)
				*running = 0;
#endif
		}
		else if (EVENT.type == SDL_KEYUP)
		{
			keyup(game, EVENT.key.keysym.sym);
		}
		else if (EVENT.type == SDL_CONTROLLERBUTTONDOWN)
		{
			controller_keydown(game, EVENT.cbutton.button);
		}
		else if (EVENT.type == SDL_CONTROLLERBUTTONUP)
		{
			controller_keyup(game, EVENT.cbutton.button);
		}
		else if (EVENT.type == SDL_QUIT)
		{
			*running = 0;
		}
	}
}
