#include <SDLRaycaster.h>

static void	load_texture(t_game *game, const char *path, t_texture *dest)
{
	SDL_Surface	*surface;
	SDL_Surface	*converted;

	surface = IMG_Load(path);
	if (!surface)
	{
		printf("IMG_Load Error: %s\n", IMG_GetError());
		cleanup(game);
	}
	converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(surface);
	if (!converted)
	{
		printf("SDL_ConvertSurfaceFormat Error: %s\n", SDL_GetError());
		cleanup(game);
	}
	surface = converted;
	dest->texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (!dest->texture)
	{
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
		cleanup(game);
	}
	dest->width = surface->w;
	dest->height = surface->h;
	dest->pixels = malloc(dest->width * dest->height * sizeof(Uint32));
	if (!dest->pixels)
	{
		printf("Memory allocation failed for texture pixels\n");
		SDL_FreeSurface(surface);
		cleanup(game);
	}
	memcpy(dest->pixels, surface->pixels, dest->width * dest->height * sizeof(Uint32));
	SDL_FreeSurface(surface);
}

void	load_textures(t_game *game)
{
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("IMG_Init Error: %s\n", IMG_GetError());
		cleanup(game);
	}
	game->textures.screen_texture = SDL_CreateTexture
		(
			game->renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			TEXTURE_WIDTH,
			TEXTURE_HEIGHT
			);
	if (!game->textures.screen_texture)
	{
		printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
		cleanup(game);
	}
	load_texture(game, "./assets/textures/png/wall.png", &game->textures.wall);
	load_texture(game, "./assets/textures/png/wall_dark.png", &game->textures.wall_dark);
	load_texture(game, "./assets/textures/png/ceiling.png", &game->textures.ceiling);
	load_texture(game, "./assets/textures/png/ceiling_dark.png", &game->textures.ceiling_dark);
	load_texture(game, "./assets/textures/png/ceiling_darker.png", &game->textures.ceiling_darker);
	load_texture(game, "./assets/textures/png/floor.png", &game->textures.floor);
	load_texture(game, "./assets/textures/png/floor_light.png", &game->textures.floor_light);
	load_texture(game, "./assets/textures/png/floor_lighter.png", &game->textures.floor_lighter);
}
