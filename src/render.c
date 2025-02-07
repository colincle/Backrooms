/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	draw_minimap(t_game *game)
{
	int			size;
	t_float_xy	minimap_corner;
	t_float_xy	start;
	int			x, y;
	float		pixels_per_unit;

	start.x = PLAYER[LEVEL]->x - 10;
	start.y = PLAYER[LEVEL]->y - 10;
	minimap_corner.y = WIND_HEIGHT / 30;
	minimap_corner.x = minimap_corner.y;
	size = WIND_WIDTH / 8;
	pixels_per_unit = size / 10.0f;

	y = minimap_corner.y;
	while (y < minimap_corner.y + size)
	{
		x = minimap_corner.x;
		while (x < minimap_corner.x + size)
		{
			int map_x = (int)floorf(start.x + (x - minimap_corner.x) / pixels_per_unit);
			int map_y = (int)floorf(start.y + (y - minimap_corner.y) / pixels_per_unit);

			if (MAPS[LEVEL][map_y] && MAPS[LEVEL][map_y][map_x])
			{
				if (MAPS[LEVEL][map_y][map_x] == EMPTY)
					SDL_SetRenderDrawColor(RENDERER, 255, 255, 0, 255);
				if (MAPS[LEVEL][map_y][map_x] == WALL)
					SDL_SetRenderDrawColor(RENDERER, 128, 128, 128, 255);
				if (MAPS[LEVEL][map_y][map_x] == DOOR_CLOSED)
					SDL_SetRenderDrawColor(RENDERER, 139, 69, 19, 255);
				SDL_RenderDrawPoint(RENDERER, x, y);
			}
			x++;
		}
		y++;
	}
	SDL_RenderPresent(RENDERER);
}

void	render_next_frame(t_game *game)
{
	if (SHOW_MINIMAP)
		draw_minimap(game);
}

	// Set the drawing color to red (RGBA: 255, 0, 0, 255)
	// SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

	// Clear the screen and fill it with the current draw color (red)
	// SDL_RenderClear(ren);

	// Draw a single pixel at coordinates (x, y) using the current draw color
	// SDL_RenderDrawPoint(ren, x, y);

	// Update the screen to display the drawn pixel
	// SDL_RenderPresent(ren);
