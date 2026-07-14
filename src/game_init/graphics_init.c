#include <SDLRaycaster.h>

// The web build draws into a fixed-size canvas, and requestAnimationFrame
// already syncs presentation to the display.
#ifdef __EMSCRIPTEN__
# define WINDOW_WIDTH	WEB_WIDTH
# define WINDOW_HEIGHT	WEB_HEIGHT
# define WINDOW_FLAGS	0
# define VSYNC_FLAG		0
#else
# define WINDOW_WIDTH	800
# define WINDOW_HEIGHT	600
# define WINDOW_FLAGS	SDL_WINDOW_FULLSCREEN_DESKTOP
# define VSYNC_FLAG		SDL_RENDERER_PRESENTVSYNC
#endif

void	init_graphics(t_game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		cleanup(game);
	}
	WINDOW = SDL_CreateWindow("SDLRaycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
	if (!WINDOW)
	{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		cleanup(game);
	}
	if (VSYNC)
		RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | VSYNC_FLAG);
	else
		RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
	if (!RENDERER)
	{
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		cleanup(game);
	}
	SDL_GetWindowSize(WINDOW, &WIND_WIDTH, &WIND_HEIGHT);
	TEXTURE_WIDTH = WIND_WIDTH / DOWNSCALE;
	TEXTURE_HEIGHT = WIND_HEIGHT / DOWNSCALE;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_StopTextInput();
}

void	pixel_buffers_init(t_game *game)
{
	game->z_buffer = calloc(TEXTURE_HEIGHT * TEXTURE_WIDTH, sizeof(float));
	if (!game->z_buffer)
		cleanup(game);
	game->screen = calloc(TEXTURE_HEIGHT * TEXTURE_WIDTH, sizeof(float));
	if (!game->z_buffer)
		cleanup(game);
}
