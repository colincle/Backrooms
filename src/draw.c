/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

void	draw_column(SDL_Renderer *renderer, int x, int yStart, int yEnd, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x, yStart, x, yEnd);
}

SDL_Color	int_to_color(Uint32 color)
{
	SDL_Color	sdlColor;

	sdlColor.r = (color >> 24) & 0xFF;
	sdlColor.g = (color >> 16) & 0xFF;
	sdlColor.b = (color >> 8) & 0xFF;
	sdlColor.a = (color) & 0xFF;
	return (sdlColor);
}

void	draw_cercle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
	int	x = 0;
	int	y = radius;
	int	d = 3 - (2 * radius);

	while (x <= y)
	{
		SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
		if (d < 0)
		{
			d += (4 * x) + 6;
		}
		else
		{
			d += (4 * (x - y)) + 10;
			y--;
		}
		x++;
	}
}
