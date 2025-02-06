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
#define FPS_CAP        60

// DEBUG
# define PRINT_ENTITIES FALSE
# define PRINT_MAPS FALSE

# ifndef DEBUG
#  define DEBUG FALSE
# endif

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

// FRAMES
#define FRAME_DELAY    (1000.0f / FPS_CAP)

// OTHERS
# define TRUE			1
# define FALSE			0
# define NUMBER_OF_MAPS	5
# define BUFFER_SIZE 64

//==============================STRUCTS
typedef struct s_int_xy
{
	int	x;
	int	y;
}			t_int_xy;

typedef struct s_float_xy
{
	float	x;
	float	y;
}			t_float_xy;

typedef struct s_entity_state
{
	float		x;
	float		y;
	t_float_xy	dir;
}			t_entity_state;

typedef struct s_entity_start
{
	int				x;
	int				y;
	t_float_xy		dir;

}			t_entity_start;

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
	t_entity_start	**player_start;
	t_entity_state	*player;
	t_entity_start	***enemy_start;
	t_entity_state	**enemy;
	t_frames		*frames;
	float			fps;
	char 			***maps;
}			t_game;

# define WINDOW game->window
# define RENDERER game->renderer
# define MAPS game->maps
# define EVENT game->event
# define FPS game->fps
# define FRAME_START game->frames->frame_start
# define FRAME_TIME game->frames->frame_time

//==============================FUNCTIONS
// CHAPTERS
void	chapter_1(t_game *game, int *i, int *running);
void	chapter_2(t_game *game, int *i, int *running);
void	chapter_3(t_game *game, int *i, int *running);
void	chapter_4(t_game *game, int *i, int *running);
void	chapter_5(t_game *game, int *i, int *running);

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

// UTILS
char	*get_next_line(int fd);
char 	*strjoin(const char *s1, const char *s2);

#endif
