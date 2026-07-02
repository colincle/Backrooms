#include <SDLRaycaster.h>

void	sound_init(t_game *game)
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		fprintf(stderr, "SDL_mixer Error: %s\n", Mix_GetError());
		cleanup(game);
	}
	SOUNDS.running = Mix_LoadWAV("./assets/sounds/running.wav");
	if (!SOUNDS.running)
	{
		fprintf(stderr, "Failed to load sound effect: %s\n", Mix_GetError());
		cleanup(game);
	}
	SOUNDS.walking = Mix_LoadWAV("./assets/sounds/walking.wav");
	if (!SOUNDS.walking)
	{
		fprintf(stderr, "Failed to load sound effect: %s\n", Mix_GetError());
		cleanup(game);
	}
	Mix_Volume(1, 64);
	Mix_Volume(2, 64);
}
