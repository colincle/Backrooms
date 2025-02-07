/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	move_player(t_game *game, int key)
{
	if (key == W)
	{
		PLAYER[LEVEL]->x = PLAYER[LEVEL]->x + (PLAYER[LEVEL]->dir.x * (PLAYER_SPEED) * (1.0 / FPS));
		PLAYER[LEVEL]->y = PLAYER[LEVEL]->y + (PLAYER[LEVEL]->dir.y * (PLAYER_SPEED) * (1.0 / FPS));
	}
	if (key == S)
	{
		PLAYER[LEVEL]->x = PLAYER[LEVEL]->x - (PLAYER[LEVEL]->dir.x * (PLAYER_SPEED) * (1.0 / FPS));
		PLAYER[LEVEL]->y = PLAYER[LEVEL]->y - (PLAYER[LEVEL]->dir.y * (PLAYER_SPEED) * (1.0 / FPS));
	}
	printf("x %f y %f%c------------------------%c", PLAYER[LEVEL]->x, PLAYER[LEVEL]->y, 10, 10); fflush(stdout); //debug
}

void	rotate_player(t_game *game, int key)
{
	float old_dir_x;
    float length;

	old_dir_x = PLAYER[LEVEL]->dir.x;
    length = sqrtf(PLAYER[LEVEL]->dir.x * PLAYER[LEVEL]->dir.x + PLAYER[LEVEL]->dir.y * PLAYER[LEVEL]->dir.y);
	if (key == D)
	{
		PLAYER[LEVEL]->dir.x = PLAYER[LEVEL]->dir.x - PLAYER[LEVEL]->dir.y * PLAYER_ROTATION_SPEED;
		PLAYER[LEVEL]->dir.y = PLAYER[LEVEL]->dir.y + old_dir_x * PLAYER_ROTATION_SPEED;
	}
	if (key == A)
	{
		PLAYER[LEVEL]->dir.x = PLAYER[LEVEL]->dir.x + PLAYER[LEVEL]->dir.y * PLAYER_ROTATION_SPEED;
		PLAYER[LEVEL]->dir.y = PLAYER[LEVEL]->dir.y - old_dir_x * PLAYER_ROTATION_SPEED;
	}
    if (length > 0) {
        PLAYER[LEVEL]->dir.x /= length;
        PLAYER[LEVEL]->dir.y /= length;
    }
}

void	update_player(t_game *game)
{
	if (KEYS[W])
		move_player(game, W);
	if (KEYS[S])
		move_player(game, S);
	if (KEYS[A])
		rotate_player(game, A);
	if (KEYS[D])
		rotate_player(game, D);
}

void	update_entities(t_game *game)
{
	update_player(game);
}