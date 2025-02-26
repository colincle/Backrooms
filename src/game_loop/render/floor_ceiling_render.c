/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	draw_floor_tile(t_game *game, t_floor_ceiling *f, t_rendering_threads *thread, char type)
{
	int		px, cell_x, cell_y, floor_tx, floor_ty;
	Uint32	floor_color;
	float	floor_x, floor_y, step_x, step_y, row_distance, pos_z;
	int		y, x, p, row_start;
	double	height;

	height = (type == WALL_0) * 0.402 + (type == WALL_1) * 0.202
		+ (type == WALL_2) * 0.0 + (type == WALL_3) * -0.200;
	pos_z = height * WIND_HEIGHT + (PLAYER_HEIGHT / 2);
	f->floor_pixels = game->textures.floor_light.pixels;
	y = f->horizon - 1;
	while (++y < WIND_HEIGHT)
	{
		p = y - (WIND_HEIGHT / 2) - CAM_SHIFT;
		row_distance = (pos_z / p) * 2;
		step_x = row_distance * (f->ray_dir_x_1 - f->ray_dir_x_0) / WIND_WIDTH;
		step_y = row_distance * (f->ray_dir_y_1 - f->ray_dir_y_0) / WIND_WIDTH;
		floor_x = PLAYER_X + row_distance * f->ray_dir_x_0
			+ thread->start * step_x;
		floor_y = PLAYER_Y + row_distance * f->ray_dir_y_0
			+ thread->start * step_y;
		row_start = y * WIND_WIDTH;
		x = thread->start - 1;
		while (++x < thread->end)
		{
			cell_x = (int)floor_x;
			cell_y = (int)floor_y;
			if (cell_x >= 0 && cell_y >= 0 && cell_x < MAP_WIDTH
				&& cell_y < MAP_HEIGHT && MAPS[LEVEL][cell_y][cell_x] == type)
			{
				floor_tx = ((int)(game->textures.floor_light.width
							* (floor_x - cell_x)))
					& (game->textures.floor_light.width - 1);
				floor_ty = ((int)(game->textures.floor_light.height
							* (floor_y - cell_y)))
					& (game->textures.floor_light.height - 1);
				floor_color = f->floor_pixels[game->textures.floor_light.width
					* floor_ty + floor_tx] | 0x010101;
				px = x;
				if (check_z_buffer(game, row_start + px, row_distance))
					f->pixels[row_start + px] = floor_color;
			}
			floor_x += step_x;
			floor_y += step_y;
		}
	}
}

static void	draw_ceiling_tile(t_game *game, t_floor_ceiling *f, t_rendering_threads *thread, char type)
{
	int		px, cell_x, cell_y, ceiling_tx, ceiling_ty;
	int		y, x, p, row_start, pixel_index;
	Uint32	ceiling_color;
	float	ceiling_x, ceiling_y, step_x, step_y, row_distance, pos_z;
	double	height;

	height = (type == WALL_5) * 0.400 + (type == WALL_6) * 0.202
		+ (type == WALL_7) * 0.0 + (type == WALL_8) * -0.200;
	pos_z = height * WIND_HEIGHT - (PLAYER_HEIGHT / 2);
	f->ceiling_pixels = game->textures.ceiling_dark.pixels;
	y = -1;
	while (++y < f->horizon)
	{
		p = (WIND_HEIGHT / 2) - y + CAM_SHIFT;
		row_distance = (pos_z / p) * 2;
		step_x = row_distance * (f->ray_dir_x_1 - f->ray_dir_x_0) / WIND_WIDTH;
		step_y = row_distance * (f->ray_dir_y_1 - f->ray_dir_y_0) / WIND_WIDTH;
		ceiling_x = PLAYER_X + row_distance * f->ray_dir_x_0
			+ thread->start * step_x;
		ceiling_y = PLAYER_Y + row_distance * f->ray_dir_y_0
			+ thread->start * step_y;
		row_start = y * WIND_WIDTH;
		x = thread->start - 1;
		while (++x < thread->end)
		{
			cell_x = (int)ceiling_x;
			cell_y = (int)ceiling_y;
			if (cell_x >= 0 && cell_y >= 0 && cell_x < MAP_WIDTH
				&& cell_y < MAP_HEIGHT && MAPS[LEVEL][cell_y][cell_x] == type)
			{
				ceiling_tx = ((int)(game->textures.ceiling_dark.width
							* (ceiling_x - cell_x)))
					& (game->textures.ceiling_dark.width - 1);
				ceiling_ty = ((int)(game->textures.ceiling_dark.height
							* (ceiling_y - cell_y)))
					& (game->textures.ceiling_dark.height - 1);
				ceiling_color = f->ceiling_pixels[game->textures.ceiling_dark.width
					* ceiling_ty + ceiling_tx] | 0x010101;
				px = x;
				pixel_index = row_start + px;
				if (check_z_buffer(game, pixel_index, row_distance))
					f->pixels[pixel_index] = ceiling_color;
			}
			ceiling_x += step_x;
			ceiling_y += step_y;
		}
	}
}

void	cast_floor_and_ceiling(t_game *game, t_floor_ceiling *f, t_rendering_threads *thread)
{
	int		px, py, cell_x, cell_y;
	int		floor_tx, floor_ty, ceiling_tx, ceiling_ty;
	float	floor_x, floor_y, ceiling_x, ceiling_y;
	float	step_x, step_y, row_distance, pos_z;
	int		y, x, p;
	Uint32	ceiling_color;
	Uint32	floor_color;

	f->horizon = (WIND_HEIGHT / 2) + CAM_SHIFT;
	f->ceiling_pixels = game->textures.ceiling.pixels;
	f->floor_pixels = game->textures.floor.pixels;
	f->ray_dir_x_0 = PLAYER_DIR_X - PLAYER_CAM_X;
	f->ray_dir_y_0 = PLAYER_DIR_Y - PLAYER_CAM_Y;
	f->ray_dir_x_1 = PLAYER_DIR_X + PLAYER_CAM_X;
	f->ray_dir_y_1 = PLAYER_DIR_Y + PLAYER_CAM_Y;
	pos_z = 0.5 * WIND_HEIGHT - (PLAYER_HEIGHT / 2);
	for (y = 0; y < f->horizon; y += 1)
	{
		p = (WIND_HEIGHT / 2) - y + CAM_SHIFT;
		row_distance = (pos_z / p) * 2;
		step_x = row_distance * (f->ray_dir_x_1 - f->ray_dir_x_0) / WIND_WIDTH;
		step_y = row_distance * (f->ray_dir_y_1 - f->ray_dir_y_0) / WIND_WIDTH;
		ceiling_x = PLAYER_X + row_distance * f->ray_dir_x_0 + thread->start * step_x;
		ceiling_y = PLAYER_Y + row_distance * f->ray_dir_y_0 + thread->start * step_y;
		for (x = thread->start; x < thread->end; x += 1)
		{
			cell_x = (int)ceiling_x;
			cell_y = (int)ceiling_y;
			if (cell_x >= 0 && cell_y >= 0 && cell_x < MAP_WIDTH
				&& cell_y < MAP_HEIGHT
				&& ((MAPS[LEVEL][cell_y][cell_x] == EMPTY || MAPS[LEVEL][cell_y][cell_x] == TRIGGER)
				|| IS_HALF_BLOCK_UP(MAPS[LEVEL][cell_y][cell_x])))
			{
				if (f->ceiling_pixels)
				{
					ceiling_tx = ((int)(game->textures.ceiling.width * (ceiling_x - cell_x)))
						& (game->textures.ceiling.width - 1);
					ceiling_ty = ((int)(game->textures.ceiling.height * (ceiling_y - cell_y)))
						& (game->textures.ceiling.height - 1);
					ceiling_color = f->ceiling_pixels[game->textures.ceiling.width * ceiling_ty + ceiling_tx];
					for (int dy = 0; dy < 1; dy++)
					{
						for (int dx = 0; dx < 1; dx++)
						{
							px = x + dx;
							py = y + dy;
							if (px >= thread->start && px < thread->end && py >= 0 && py < WIND_HEIGHT)
							{
								if (check_z_buffer(game, py * WIND_WIDTH + px, row_distance))
									f->pixels[py * WIND_WIDTH + px] = ceiling_color;
							}
						}
					}
				}
			}
			ceiling_x += step_x;
			ceiling_y += step_y;
		}
	}
	pos_z = 0.5 * WIND_HEIGHT + (PLAYER_HEIGHT / 2);
	for (y = f->horizon; y < WIND_HEIGHT; y++)
	{
		p = y - (WIND_HEIGHT / 2) - CAM_SHIFT;
		row_distance = (pos_z / p) * 2;
		step_x = row_distance * (f->ray_dir_x_1 - f->ray_dir_x_0) / WIND_WIDTH;
		step_y = row_distance * (f->ray_dir_y_1 - f->ray_dir_y_0) / WIND_WIDTH;
		floor_x = PLAYER_X + row_distance * f->ray_dir_x_0 + thread->start * step_x;
		floor_y = PLAYER_Y + row_distance * f->ray_dir_y_0 + thread->start * step_y;
		for (x = thread->start; x < thread->end; x++)
		{
			cell_x = (int)floor_x;
			cell_y = (int)floor_y;
			if (cell_x >= 0 && cell_y >= 0 && cell_x < MAP_WIDTH
				&& cell_y < MAP_HEIGHT
				&& ((MAPS[LEVEL][cell_y][cell_x] == EMPTY || MAPS[LEVEL][cell_y][cell_x] == TRIGGER)
				|| IS_HALF_BLOCK_DOWN(MAPS[LEVEL][cell_y][cell_x])))
			{
				if (f->floor_pixels)
				{
					floor_tx = ((int)(game->textures.floor.width * (floor_x - cell_x)))
						& (game->textures.floor.width - 1);
					floor_ty = ((int)(game->textures.floor.height * (floor_y - cell_y)))
						& (game->textures.floor.height - 1);
					floor_color = f->floor_pixels[game->textures.floor.width * floor_ty + floor_tx];
					for (int dy = 0; dy < 1; dy++)
					{
						for (int dx = 0; dx < 1; dx++)
						{
							px = x + dx;
							py = y + dy;
							if (px >= thread->start && px < thread->end && py < WIND_HEIGHT)
							{
								if (check_z_buffer(game, py * WIND_WIDTH + px, row_distance))
								{
									set_z_buffer(game, row_distance, py * WIND_WIDTH + px);
									f->pixels[py * WIND_WIDTH + px] = floor_color;
								}
							}
						}
					}
				}
			}
			floor_x += step_x;
			floor_y += step_y;
		}
	}
}

void	cast_offset_height_floor_and_ceiling(t_game *game, t_floor_ceiling *f, t_rendering_threads *thread)
{
	if (PLAYER_HEIGHT >= -620 && PLAYER_HEIGHT <= -430)
	{
		draw_floor_tile(thread->game, f, thread, WALL_0);
		draw_ceiling_tile(thread->game, f, thread, WALL_5);
		draw_ceiling_tile(thread->game, f, thread, WALL_6);
		draw_ceiling_tile(thread->game, f, thread, WALL_7);
		draw_ceiling_tile(thread->game, f, thread, WALL_8);
	}
	else if (PLAYER_HEIGHT > -430 && PLAYER_HEIGHT <= 0)
	{
		draw_floor_tile(thread->game, f, thread, WALL_0);
		draw_floor_tile(thread->game, f, thread, WALL_1);
		draw_ceiling_tile(thread->game, f, thread, WALL_5);
		draw_ceiling_tile(thread->game, f, thread, WALL_6);
		draw_ceiling_tile(thread->game, f, thread, WALL_7);
	}
	else if (PLAYER_HEIGHT > 0 && PLAYER_HEIGHT <= 430)
	{
		draw_floor_tile(thread->game, f, thread, WALL_0);
		draw_floor_tile(thread->game, f, thread, WALL_1);
		draw_floor_tile(thread->game, f, thread, WALL_2);
		draw_ceiling_tile(thread->game, f, thread, WALL_5);
		draw_ceiling_tile(thread->game, f, thread, WALL_6);
	}
	else if (PLAYER_HEIGHT > 430)
	{
		draw_floor_tile(thread->game, f, thread, WALL_0);
		draw_floor_tile(thread->game, f, thread, WALL_1);
		draw_floor_tile(thread->game, f, thread, WALL_2);
		draw_floor_tile(thread->game, f, thread, WALL_3);
		draw_ceiling_tile(thread->game, f, thread, WALL_5);
	}
}
