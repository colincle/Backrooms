/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

#include <SDL2/SDL.h>

void draw_cercle(SDL_Renderer *renderer, int centerX, int centerY, int radius) 
{
	int x = 0;
	int y = radius;
	int d = 3 - (2 * radius);

	while (x <= y) {
		SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);

		if (d < 0) {
			d += (4 * x) + 6;
		} else {
			d += (4 * (x - y)) + 10;
			y--;
		}
		x++;
	}
}

void draw_minimap(t_game *game)
{
    int x, y;
    // Set starting position for rendering at the top-left (0,0)
    // Loop through the entire map (until we hit NULL)
    for (y = 0; MAPS[LEVEL][y] != NULL; y++)
    {
        for (x = 0; MAPS[LEVEL][y][x] != '\0'; x++)
        {
            SDL_Rect cell = {x * MINIMAP_BLOCK_SIZE, y * MINIMAP_BLOCK_SIZE, MINIMAP_BLOCK_SIZE, MINIMAP_BLOCK_SIZE };

            // Set the correct color based on the map cell type
            if (MAPS[LEVEL][y][x] == EMPTY)
                SDL_SetRenderDrawColor(RENDERER, 255, 255, 0, 255);  // Yellow for empty
            else if (MAPS[LEVEL][y][x] == WALL)
                SDL_SetRenderDrawColor(RENDERER, 128, 128, 128, 255); // Gray for walls
            else if (MAPS[LEVEL][y][x] == DOOR_CLOSED)
                SDL_SetRenderDrawColor(RENDERER, 139, 69, 19, 255);   // Brown for doors

            SDL_RenderFillRect(RENDERER, &cell);
        }
    }

    SDL_SetRenderDrawColor(RENDERER, 255, 0, 0, 255); // Red for player
    draw_cercle(RENDERER, PLAYER[LEVEL]->x * MINIMAP_BLOCK_SIZE, PLAYER[LEVEL]->y * MINIMAP_BLOCK_SIZE, MINIMAP_BLOCK_SIZE / 4);
	SDL_RenderDrawLine(RENDERER, PLAYER[LEVEL]->x * MINIMAP_BLOCK_SIZE, PLAYER[LEVEL]->y * MINIMAP_BLOCK_SIZE, 
	PLAYER[LEVEL]->x * MINIMAP_BLOCK_SIZE + (PLAYER[LEVEL]->dir.x * MINIMAP_BLOCK_SIZE), 
	PLAYER[LEVEL]->y * MINIMAP_BLOCK_SIZE + (PLAYER[LEVEL]->dir.y * MINIMAP_BLOCK_SIZE));

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
