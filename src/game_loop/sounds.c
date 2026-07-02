#include <SDLRaycaster.h>

void	sounds(t_game *game)
{
	if (MOVING == RUNNING)
	{
		if (Mix_Playing(2))
			Mix_HaltChannel(2);
		if (!Mix_Playing(1))
		{
			Mix_PlayChannel(1, SOUNDS.running, 0);
		}
	}
	if (MOVING == WALKING)
	{
		if (Mix_Playing(1))
			Mix_HaltChannel(1);
		if (!Mix_Playing(2))
		{
			Mix_PlayChannel(2, SOUNDS.walking, 0);
		}
	}
	if (MOVING == STILL)
	{
		Mix_HaltChannel(-1);
	}
}
