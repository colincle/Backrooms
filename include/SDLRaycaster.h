#ifndef SDLRAYCASTER_H
# define SDLRAYCASTER_H

# include <SDL2/SDL_image.h>
# include <SDL2/SDL_mixer.h>
# include <SDL2/SDL.h>
# include <sys/types.h>
# include <sys/sysctl.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <fcntl.h>

//==============================MACROS
// SETTINGS
# define DOWNSCALE			1
# define FOV				53
# define FPS_CAP			121
# define DEFAULT_SPEED		2
# define MOUSE_SENSIT		0.03
# define JOY_SENSIT			2
# define GRAVITY 			80
# define COLLISION_RADIUS	0.3f
# define VSYNC				TRUE

// DEBUG
# define START_LEVEL		0
# define SHOW_MINIMAP 		FALSE
# define SHOW_FPS			FALSE
# define PRINT_ENTITIES 	FALSE
# define PRINT_MAPS 		FALSE
# define SHOW_POSITION		FALSE
# define SHOW_CAM_PLANE		FALSE
# define SHOW_DIRECTION		FALSE
# define MINIMAP_BLOCK		5

// MAP CHARACTERS
# define WALL				'#'
// Sticking out of the floor
# define WALL_0				'0'
# define WALL_1				'1'
# define WALL_2				'2'
# define WALL_3				'3'
# define WALL_4				'4'
# define IS_HALF_BLOCK_UP(c) ((c) == WALL_0 || (c) == WALL_1 || (c) == WALL_2 || (c) == WALL_3 || (c) == WALL_4)
// Sticking out of the ceiling
# define WALL_5				'5'
# define WALL_6				'6'
# define WALL_7				'7'
# define WALL_8				'8'
# define WALL_9				'9'
# define IS_HALF_BLOCK_DOWN(c) ((c) == WALL_5 || (c) == WALL_6 || (c) == WALL_7 || (c) == WALL_8 || (c) == WALL_9)
# define VOID				'.'
# define EMPTY				' '
# define P_NORTH			'N'
# define P_SOUTH			'S'
# define P_EAST				'E'
# define P_WEST				'W'
# define E_NORTH			'n'
# define E_SOUTH			's'
# define E_EAST				'e'
# define E_WEST				'w'
# define DOOR_CLOSED		'D'
# define DOOR_OPENED		'U'
# define DOOR_CLOSING		'd'
# define DOOR_OPENING		'u'
# define TRIGGER			'T'
# define HOLE				':'
# define PILLAR				';'

// KEYS
# define W					0
# define A					1
# define S					2
# define D					3
# define SHIFT				4
# define C					5
# define HOW_MANY_KEYS		6

// KEYS STATES
# define PRESS				1
# define HOLD				2

// PLAYER MOVEMENT

# define STILL				0
# define RUNNING			1
# define WALKING			2
# define JUMP_UP			1
# define JUMP_DOWN			2
# define NO_JUMP			0
# define CROUCHING			1
# define CRAWLING			2
# define STANDING			0

// OTHERS
# define TRUE				1
# define FALSE				0
# define NUMBER_OF_MAPS		5
# define BUFFER_SIZE		64

//==============================STRUCTS

typedef struct s_point
{
	int						x;
	int						y;
}							t_point;

typedef struct s_float_xy
{
	float					x;
	float					y;
}							t_float_xy;

typedef struct s_entity
{
	float					x;
	float					y;
	t_float_xy				spawn;
	t_point					spawn_dir;
	t_float_xy				dir;
	t_float_xy				cam;
	int						camera_shift;
	int						feet_touch;
	int						head_touch;
	int						jumping;
	int						crouching;
	int						crouch_lock;
	int						stand_lock;
	int						jump_lock;
	int						falling;
	int						speed;
}							t_player;
typedef struct s_enemy
{
	float					x;
	float					y;
	t_float_xy				dir;
}							t_enemy;

typedef struct s_heights
{
	int						hole;
	int						empty;
	int						wall_0;
	int						wall_1;
	int						wall_2;
	int						wall_3;
	int						wall_4;
	int						wall_5;
	int						wall_6;
	int						wall_7;
	int						wall_8;
	int						wall_9;
	int						crouch;
	int						crawl;
	int						jump;
	int						height_cap;
	int						empty_height_cap;
	int						wall_6_height_cap;
	int						wall_5_height_cap;
	int						feet_height;
	int						eye_height;
	int						player_height;
}							t_heights;

typedef struct s_frames
{
	int						frame_time;
	Uint32					frame_start;
}							t_frames;

typedef struct s_sounds
{
	Mix_Chunk				*running;
	Mix_Chunk				*walking;
}							t_sounds;

typedef struct s_floor_ceiling
{
	Uint32					*pixels;
	Uint32					*floor_pixels;
	Uint32					*ceiling_pixels;
	float					ray_dir_x_0;
	float					ray_dir_y_0;
	float					ray_dir_x_1;
	float					ray_dir_y_1;
	int						horizon;
}							t_floor_ceiling;

struct						s_mini_ray;

typedef struct s_raycaster
{
	int						detected;
	int						first_hole;
	int						x;
	int						side;
	float					pos_x;
	float					pos_y;
	int						map_x;
	int						map_y;
	float					cam_x;
	int						step_x;
	float					offset;
	int						step_y;
	int						wall_dir;
	float					ray_dir_x;
	float					ray_dir_y;
	float					wall_hit_y;
	float					wall_hit_x;
	float					side_dist_x;
	float					side_dist_y;
	float					delta_dist_x;
	float					delta_dist_y;
	float					perp_wall_dist;
	struct s_mini_ray		*mini_ray;
}							t_raycaster;

typedef struct s_mini_ray
{
	t_raycaster				ray;
	struct s_mini_ray		*next;
	struct s_mini_ray		*prev;
}							t_mini_ray;

typedef struct s_input
{
	int						*keys;
	Uint32					c_hold;
	int						c_state;
	int						mouse_x_rel;
	int						mouse_y_rel;
	float					joystick_velocity_x;
	float					joystick_velocity_y;
	float					joystick_rotation_x;
	float					joystick_rotation_y;
}							t_input;

typedef struct s_texture
{
	SDL_Texture				*texture;
	Uint32					*pixels;
	int						width;
	int						height;
}							t_texture;

typedef struct s_textures
{
	SDL_Texture				*screen_texture;
	t_texture				wall;
	t_texture				wall_dark;
	t_texture				ceiling;
	t_texture				ceiling_dark;
	t_texture				ceiling_darker;
	t_texture				floor;
	t_texture				floor_light;
	t_texture				floor_lighter;
}							t_textures;

typedef struct s_game
{
	float					fps;
	double					frame_time;
	SDL_Event				event;
	t_input					input;
	int						level;
	SDL_Window				*window;
	char					***maps;
	t_point					*maps_sizes;
	t_player				**player;
	t_enemy					***enemy;
	int						moving;
	t_textures				textures;
	SDL_Renderer			*renderer;
	int						texture_width;
	int						texture_height;
	int						wind_width;
	int						wind_height;
	float					*z_buffer;
	t_sounds				sounds;
	Uint32					*screen;
	t_heights				heights;
	int						P_cores;
}							t_game;

typedef struct s_rendering_threads
{
	int						total_threads;
	int						thread_id;
	int						start;
	int						end;
	t_game					*game;
}							t_rendering_threads;

// STRUCT ACCESS MACROS
# define CAM_SHIFT			game->player[LEVEL]->camera_shift
# define JUMP				game->player[LEVEL]->jumping
# define JUMP_LOCK			game->player[LEVEL]->jump_lock
# define CROUCH				game->player[LEVEL]->crouching
# define CROUCH_LOCK		game->player[LEVEL]->crouch_lock
# define STAND_LOCK			game->player[LEVEL]->stand_lock
# define FALLING			game->player[LEVEL]->falling
# define FEET_TOUCH			game->player[LEVEL]->feet_touch
# define HEAD_TOUCH			game->player[LEVEL]->head_touch
# define PLAYER_X			game->player[(LEVEL)]->x
# define PLAYER_Y			game->player[(LEVEL)]->y
# define PLAYER_DIR_X		game->player[(LEVEL)]->dir.x
# define PLAYER_DIR_Y		game->player[(LEVEL)]->dir.y
# define PLAYER_CAM_X		game->player[(LEVEL)]->cam.x
# define PLAYER_CAM_Y		game->player[(LEVEL)]->cam.y
# define PLAYER_SPEED		game->player[(LEVEL)]->speed
# define MAP_WIDTH			game->maps_sizes[(LEVEL)].x
# define MAP_HEIGHT			game->maps_sizes[(LEVEL)].y
# define PLAYER				game->player
# define MOUSE_X			game->input.mouse_x_rel
# define MOUSE_Y			game->input.mouse_y_rel
# define JOYSTICK_ROT_X		game->input.joystick_rotation_x
# define JOYSTICK_ROT_Y		game->input.joystick_rotation_y
# define JOYSTICK_X			game->input.joystick_velocity_x
# define JOYSTICK_Y			game->input.joystick_velocity_y
# define KEYS				game->input.keys
# define FPS				game->fps
# define FRAME_TIME			game->frame_time
# define MAPS				game->maps
# define LEVEL				game->level
# define EVENT				game->event
# define ENEMY				game->ENEMY
# define MOVING				game->moving
# define SOUNDS				game->sounds
# define WINDOW				game->window
# define RENDERER			game->renderer
# define TEXTURE_WIDTH		game->texture_width
# define TEXTURE_HEIGHT		game->texture_height
# define WIND_WIDTH			game->wind_width
# define WIND_HEIGHT		game->wind_height
# define HOLE_HEIGHT		game->heights.hole
# define EMPTY_HEIGHT		game->heights.empty
# define WALL_0_HEIGHT		game->heights.wall_0
# define WALL_1_HEIGHT		game->heights.wall_1
# define WALL_2_HEIGHT		game->heights.wall_2
# define WALL_3_HEIGHT		game->heights.wall_3
# define WALL_4_HEIGHT		game->heights.wall_4
# define WALL_5_HEIGHT		game->heights.wall_5
# define WALL_6_HEIGHT		game->heights.wall_6
# define WALL_7_HEIGHT		game->heights.wall_7
# define WALL_8_HEIGHT		game->heights.wall_8
# define WALL_9_HEIGHT		game->heights.wall_9
# define CROUCH_HEIGHT		game->heights.crouch
# define CRAWL_HEIGHT		game->heights.crawl
# define JUMP_HEIGHT		game->heights.jump
# define HEIGHT_CAP			game->heights.height_cap
# define EMPTY_HEIGHT_CAP	game->heights.empty_height_cap
# define W6_HEIGHT_CAP		game->heights.wall_6_height_cap
# define W5_HEIGHT_CAP		game->heights.wall_5_height_cap
# define FEET_HEIGHT		game->heights.feet_height
# define EYE_HEIGHT			game->heights.eye_height
# define PLAYER_HEIGHT		game->heights.player_height

//==============================FUNCTIONS
// controller.c
void						controller_keydown(t_game *game, SDL_GameControllerButton button);
void						controller_keyup(t_game *game, SDL_GameControllerButton button);

// keyboard.c
void						keydown(t_game *game, SDL_KeyCode code);
void						keyup(t_game *game, SDL_KeyCode code);

// debug_statements.c
void						debug_statements(t_game *game);
void						print_entities(t_game *game);
void						print_all_maps(t_game *game);

// draw.c
void						draw_column(SDL_Renderer *renderer, int x, int yStart, int yEnd, SDL_Color color);
SDL_Color					int_to_color(Uint32 color);
void						draw_cercle(SDL_Renderer *renderer, int centerX, int centerY, int radius);

// minimap.c
void						draw_minimap(t_game *game);

// entities_init.c
void						set_player_cam(t_game *game, int i);
void						entities_init(t_game *game);

// game_init.c
t_game						*init_game(void);

// get_map.c
char						*get_path(int i);
char						**get_map(char *path);

// graphics_init.c
void						init_graphics(t_game *game);
void						pixel_buffers_init(t_game *game);

// input_init.c
void						keyboard_key_states_init(t_game *game);
void						controller_init(t_game *game);
void						mouse_init(void);

// maps_init.c
void						maps_init(t_game *game);

// sound_init.c
void						sound_init(t_game *game);

// textures_init.c
void						load_textures(t_game *game);

// chapters.c
void						chapter_1(t_game *game, int *running);
void						chapter_2(t_game *game, int *running);
void						chapter_3(t_game *game, int *running);
void						chapter_4(t_game *game, int *running);
void						chapter_5(t_game *game, int *running);

// events.c
void						handle_events(t_game *game, int *running);

// frame_sync.c
void						manage_fps(t_game *game);

// floor_ceiling_render.c
void						cast_floor_and_ceiling(t_game *game, t_floor_ceiling *f, t_rendering_threads *thread);
void						cast_offset_height_floor_and_ceiling(t_game *game, t_floor_ceiling *f, t_rendering_threads *thread);

// raycasting.c
void						perform_raycaster_steps(t_raycaster *r, t_game *game);
void						init_raycaster_steps(t_raycaster *r);
void						init_raycaster(t_raycaster *r, t_game *game);

// render.c
void						render_next_frame(t_game *game);

// wall_render.c
void						half_down_block(t_game *game, t_raycaster *r, void *pixels);
void						half_block_up(t_game *game, t_raycaster *r, void *pixels);
void						hole_block(t_game *game, t_raycaster *r, void *pixels);
void						pillar_block(t_game *game, t_raycaster *r, void *pixels);
void						draw_wall(t_game *game, t_raycaster *r, void *pixels);

// z_buffer.c
void						clear_z_buffer(t_game *game);
int							check_z_buffer(t_game *game, int index, float z);
void						set_z_buffer(t_game *game, float perp_wall_dist, int index);

// sounds.c
void						sounds(t_game *game);

// camera_movements.c
void						rotate_player_mouse(t_game *game, int x);
void						rotate_player_joystick(t_game *game, float x);
void						look_up_and_down_joystick(t_game *game, float x);
void						look_up_and_down_mouse(t_game *game, int y);

// collisions.c
void						collisions(t_game *game, float new_x, float new_y);

// player_horizontal_movement.c
void						left_stick(t_game *game);
void						move_player(t_game *game, int key);
void						move_player_joystick(t_game *game, float x, float y);

// player_vertical_movement.c
void						crouch(t_game *game);
void						jump(t_game *game);
void						gravity(t_game *game);
void						update_player_height(t_game *game);

// update_entities.c
void						update_entities(t_game *game);

// cleanup.c
void						free_all(char **array);
void						cleanup(t_game *game);

// get_next_line.c
char						*get_next_line(int fd);

// misc.c
char						*strjoin(const char *s1, const char *s2);
void						free_all(char **array);

//main.c
void						quit_game(t_game *game);

#endif
