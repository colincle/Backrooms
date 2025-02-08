/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

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
