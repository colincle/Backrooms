/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	render_next_frame(t_game *game)
{
	draw_scene(game);
	draw_minimap(game);
}
