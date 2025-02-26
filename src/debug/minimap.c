/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void draw_minimap(t_game *game)
{
    int x;
    int y;

    if (!SHOW_MINIMAP)
        return;
    for (y = 0; MAPS[LEVEL][y] != NULL; y++)
    {
        for (x = 0; MAPS[LEVEL][y][x] != '\0'; x++)
        {
            SDL_Rect cell = { x * MINIMAP_BLOCK, y * MINIMAP_BLOCK, MINIMAP_BLOCK, MINIMAP_BLOCK };
            char block = MAPS[LEVEL][y][x];
            if (block == EMPTY)
                SDL_SetRenderDrawColor(RENDERER, 255, 255, 0, 255);
            else if (block >= WALL_0 && block <= WALL_4)
            {
                switch (block)
                {
                    case WALL_0: SDL_SetRenderDrawColor(RENDERER, 0, 100, 0, 255); break;       // Dark green
                    case WALL_1: SDL_SetRenderDrawColor(RENDERER, 34, 139, 34, 255); break;       // Forest green
                    case WALL_2: SDL_SetRenderDrawColor(RENDERER, 60, 179, 113, 255); break;      // Medium sea green
                    case WALL_3: SDL_SetRenderDrawColor(RENDERER, 144, 238, 144, 255); break;     // Light green
                    case WALL_4: SDL_SetRenderDrawColor(RENDERER, 152, 251, 152, 255); break;     // Pale green
                }
            }
            else if (block >= '5' && block <= '9')
            {
                switch (block)
                {
                    case '5': SDL_SetRenderDrawColor(RENDERER, 0, 0, 139, 255); break;         // Dark blue
                    case '6': SDL_SetRenderDrawColor(RENDERER, 0, 0, 205, 255); break;         // Medium blue
                    case '7': SDL_SetRenderDrawColor(RENDERER, 65, 105, 225, 255); break;      // Royal blue
                    case '8': SDL_SetRenderDrawColor(RENDERER, 100, 149, 237, 255); break;     // Cornflower blue
                    case '9': SDL_SetRenderDrawColor(RENDERER, 135, 206, 250, 255); break;     // Light sky blue
                }
            }
            else if (block == WALL)
                SDL_SetRenderDrawColor(RENDERER, 128, 128, 128, 255);
            else if (block == DOOR_CLOSED)
                SDL_SetRenderDrawColor(RENDERER, 139, 69, 19, 255);
            SDL_RenderFillRect(RENDERER, &cell);
        }
    }
    SDL_SetRenderDrawColor(RENDERER, 255, 0, 0, 255);
    draw_cercle(RENDERER, PLAYER_X * MINIMAP_BLOCK, PLAYER_Y * MINIMAP_BLOCK, MINIMAP_BLOCK / 4);
    SDL_RenderDrawLine(RENDERER, PLAYER_X * MINIMAP_BLOCK, PLAYER_Y * MINIMAP_BLOCK,
        PLAYER_X * MINIMAP_BLOCK + (PLAYER_DIR_X * MINIMAP_BLOCK),
        PLAYER_Y * MINIMAP_BLOCK + (PLAYER_DIR_Y * MINIMAP_BLOCK));
}

