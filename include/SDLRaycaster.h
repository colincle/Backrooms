#ifndef SDLRAYCASTER_H
#define SDLRAYCASTER_H

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include <SDL2/SDL_mixer.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>

//==============================MACROS
// SETTINGS
# define FPS_CAP				60
# define PLAYER_SPEED			30
# define PLAYER_ROTATION_SPEED	0.1

// DEBUG
# define PRINT_ENTITIES FALSE
# define PRINT_MAPS TRUE
# define SHOW_MINIMAP TRUE
# define MINIMAP_BLOCK_SIZE 25

// MAP CHARACTERS
# define WALL			'#'
# define VOID			'.'
# define EMPTY			' '
# define P_NORTH		'N'
# define P_SOUTH		'S'
# define P_EAST			'E'
# define P_WEST			'W'
# define E_NORTH		'n'
# define E_SOUTH		's'
# define E_EAST			'e'
# define E_WEST			'w'
# define DOOR_CLOSED	'D'
# define DOOR_OPENED	'U'
# define DOOR_CLOSING	'd'
# define DOOR_OPENING	'u'

// KEYS
# define W 0
# define A 1
# define S 2
# define D 3
# define HOW_MANY_KEYS 4

// FRAMES
#define FRAME_DELAY    (1000.0f / FPS_CAP)

// OTHERS
# define TRUE			1
# define FALSE			0
# define NUMBER_OF_MAPS	5
# define BUFFER_SIZE 64

//==============================STRUCTS

typedef struct s_float_xy
{
	float	x;
	float	y;
}			t_float_xy;

typedef struct s_entity
{
	float		x;
	float		y;
	t_float_xy	dir;
}			t_entity;

typedef struct s_frames
{
	int		frame_time;
	Uint32	frame_start;
}			t_frames;

typedef struct s_game
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Event		event;
	int				*keys;
	int				wind_height;
	int				wind_width;
	int				level;
	t_entity		**player;
	t_entity		***enemy;
	t_frames		*frames;
	char 			***maps;
	float			fps;
}			t_game;

# define WINDOW game->window
# define RENDERER game->renderer
# define EVENT game->event
# define KEYS game->keys
# define WIND_HEIGHT game->wind_height
# define WIND_WIDTH game->wind_width
# define LEVEL game->level
# define PLAYER game->player
# define ENEMY game->ENEMY
# define FPS game->fps
# define MAPS game->maps
# define FRAME_START game->frames->frame_start
# define FRAME_TIME game->frames->frame_time

//==============================FUNCTIONS
// CHAPTERS
void	chapter_1(t_game *game, int *running);
void	chapter_2(t_game *game, int *running);
void	chapter_3(t_game *game, int *running);
void	chapter_4(t_game *game, int *running);
void	chapter_5(t_game *game, int *running);

// CLEANUP
void	cleanup(t_game *game);
void 	free_all(char **array);

//DEBUG TOOLS
void	print_all_maps(t_game *game);
void 	print_entities(t_game *game);

// EVENTS
void	handle_events(t_game *game, int *running);

// GET_MAP
char	**get_map(char *path);
char	*get_path(int i);

// INIT
t_game	*game_init();

// RENDER
void	render_next_frame(t_game *game);

// UPDATE ENTITIES
void	update_entities(t_game *game);

// UTILS
char	*get_next_line(int fd);
char 	*strjoin(const char *s1, const char *s2);

#endif
