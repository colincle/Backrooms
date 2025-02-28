/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

int	set_height(t_game *game, float percentage)
{
	// Clamp percentage between 0 and 100
	if (percentage < 0.0f)
		percentage = 0.0f;
	if (percentage > 100.0f)
		percentage = 100.0f;

	// Scale percentage from 0% (ground) to 100% (twice the texture height)
	float scaled_height = (percentage / 100.0f) * (TEXTURE_HEIGHT * 2.0f);

	return (int)scaled_height;
}


static void	get_P_cores(t_game *game)
{
	int		pcores = 0;
	size_t	size = sizeof(pcores);

	if (sysctlbyname("hw.perflevel0.logicalcpu", &pcores, &size, NULL, 0) == 0)
		game->P_cores = pcores;
	else
		game->P_cores = 0;
}

static t_game	*game_struct_init(void)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	if (!game)
	{
		fprintf(stderr, "ERROR: memory allocation failed in init_game");
		exit(EXIT_FAILURE);
	}
	game->fps = 0;
	game->level = 0;
	game->moving = 0;
	game->texture_width = 0;
	game->texture_height = 0;
	game->P_cores = 0;
	game->event = (SDL_Event){0};
	game->input = (t_input){0};
	game->textures = (t_textures){0};
	game->sounds = (t_sounds){0};
	game->maps = NULL;
	game->player = NULL;
	game->enemy = NULL;
	game->window = NULL;
	game->renderer = NULL;
	game->z_buffer = NULL;
	game->screen = NULL;
	game->maps_sizes = NULL;
	keyboard_key_states_init(game);
	get_P_cores(game);
	return (game);
}

static void	heights_init(t_game *game)
{
	game->heights.empty = set_height(game, 0);
	game->heights.wall_0 = set_height(game, 10);
	game->heights.wall_1 = set_height(game, 30);
	game->heights.wall_2 = set_height(game, 50);
	game->heights.wall_3 = set_height(game, 70);
	game->heights.wall_4 = set_height(game, 80);
	game->heights.wall_5 = set_height(game, 80);
	game->heights.wall_6 = set_height(game, 70);
	game->heights.wall_7 = set_height(game, 50);
	game->heights.wall_8 = set_height(game, 30);
	game->heights.wall_9 = set_height(game, 10);
	game->heights.crouch = set_height(game, 35);
	game->heights.crawl = set_height(game, 20);
	game->heights.jump = set_height(game, 25);
	game->heights.height_cap = set_height(game, 45);
	game->heights.player_height = set_height(game, 50);
	game->heights.eye_height = set_height(game, 50);
	game->heights.feet_height = set_height(game, 0);
	
	printf("feet %d%c------------------------%c", FEET_HEIGHT, 10, 10);
	fflush(stdout); // Debug
}



t_game	*init_game(void)
{
	t_game	*game;

	game = game_struct_init();
	controller_init(game);
	init_graphics(game);
	mouse_init();
	sound_init(game);
	load_textures(game);
	maps_init(game);
	print_all_maps(game);
	entities_init(game);
	print_entities(game);
	pixel_buffers_init(game);
	heights_init(game);

	return (game);
}
