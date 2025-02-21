/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	half_down_block(t_game *game, t_raycaster *r)
{
	int			tex_x;
	int			start;
	int			block_height;
	double		wall_x;
	SDL_Rect	dest;
	SDL_Rect	src;
	int			wind_height;
	int			tex_w;
	int			tex_h;
	double		inv_perp_wall_dist;
	SDL_Texture	*wall_texture;

	SDL_SetTextureColorMod(game->textures.wall.texture, 255, 255, 255); // Reset brightness

	wind_height = WIND_HEIGHT;
	tex_w = game->textures.wall.width;
	tex_h = game->textures.wall.height;
	inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
	int line_height = (int)(wind_height * inv_perp_wall_dist); // Keep base height

	start = ((wind_height - line_height) >> 1) + CAM_SHIFT;

	// Fix scaling to have a proper min size & max cap
	double normalized = (r->detected - 5) / 4.0; // Range: 0 (min) to 1 (max)
	block_height = line_height * (0.1 + (normalized * 0.8)); // Min 10%, Max 90%

	wall_x = (r->side == 0)
		? (r->pos_y + r->perp_wall_dist * r->ray_dir_y)
		: (r->pos_x + r->perp_wall_dist * r->ray_dir_x);
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * tex_w);
	tex_x &= (tex_w - 1);

	// Darken walls that face north/south (side 1)
	if (r->side == 1)
		SDL_SetTextureColorMod(game->textures.wall.texture, 180, 180, 180);

	src.x = tex_x;
	src.y = 0;
	src.w = 1;
	src.h = tex_h;
	dest.x = r->x;
	dest.y = start;
	dest.h = block_height; // Scaled height with a proper min/max range
	dest.w = PIXEL_BLOCK;

	// Render the wall
	SDL_RenderCopy(RENDERER, game->textures.wall.texture, &src, &dest);

	// Reset texture brightness for next frame
	SDL_SetTextureColorMod(game->textures.wall.texture, 255, 255, 255);
	r->detected = -1;
}

// static t_point find_floor_end(t_game *game, t_raycaster *r)
// {
// 	int start, block_height;
// 	double inv_perp_wall_dist;
// 	int line_height;

// 	// **1️⃣ Take ONE DDA step inside the block**
// 	if (r->side_dist_x < r->side_dist_y)
// 	{
// 		r->side_dist_x += r->delta_dist_x;
// 		r->map_x += r->step_x;
// 		r->side = 0;
// 	}
// 	else
// 	{
// 		r->side_dist_y += r->delta_dist_y;
// 		r->map_y += r->step_y;
// 		r->side = 1;
// 	}
// 	ray_has_hit_wall(r);

// 	// **2️⃣ Compute perpendicular wall distance**
// 	inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
// 	line_height = (int)(WIND_HEIGHT * inv_perp_wall_dist);

// 	start = ((WIND_HEIGHT - line_height) >> 1) + CAM_SHIFT;

// 	// **3️⃣ Scale height correctly for the far side of the block**
// 	double normalized = r->detected / 4.0;  // Range: 0 (min) to 1 (max)
// 	block_height = line_height * (0.1 + (normalized * 0.8)); // Min 10%, Max 90%

// 	int inside_wall_y = start + (line_height - block_height);

// 	return ((t_point){r->x, inside_wall_y});
// }

static void half_block_up(t_game *game, t_raycaster *r)
{
	int tex_x, start, block_height;
	double wall_x;
	SDL_Rect dest, src;
	int wind_height = WIND_HEIGHT;
	int tex_w = game->textures.wall.width;
	int tex_h = game->textures.wall.height;
	double inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
	int line_height = (int)(wind_height * inv_perp_wall_dist);

	start = ((wind_height - line_height) >> 1) + CAM_SHIFT;

	// Fix scaling for proper min/max heights
	double normalized = r->detected / 4.0; // Range: 0 (min) to 1 (max)
	block_height = line_height * (0.1 + (normalized * 0.8)); // Min 10%, Max 90%

	// Adjust wall position to stick to the floor
	wall_x = (r->side == 0)
		? (r->pos_y + r->perp_wall_dist * r->ray_dir_y)
		: (r->pos_x + r->perp_wall_dist * r->ray_dir_x);
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * tex_w) & (tex_w - 1);

	// Darken walls that face north/south (side 1)
	if (r->side == 1)
		SDL_SetTextureColorMod(game->textures.wall.texture, 180, 180, 180);

	src.x = tex_x;
	src.y = 0;
	src.w = 1;
	src.h = tex_h;
	dest.x = r->x;
	dest.y = start + (line_height - block_height);
	dest.h = block_height;
	dest.w = PIXEL_BLOCK;

	// Render the half block wall
	SDL_RenderCopy(RENDERER, game->textures.wall.texture, &src, &dest);

	// Reset texture brightness
	SDL_SetTextureColorMod(game->textures.wall.texture, 255, 255, 255);
}

// static void draw_half_block_floor(t_point start, t_point end, t_game *game, t_raycaster *r)
// {
// 	if (start.y < end.y)
// 		return; // Floor is not visible, skip rendering

// 	// Get texture dimensions
// 	int tex_w = game->textures.floor.width;
// 	int tex_h = game->textures.floor.height;
// 	Uint32 *floor_pixels = game->textures.floor.pixels;

// 	// Compute world-space coordinates of the block surface
// 	double floorXWall = r->map_x + 0.5;
// 	double floorYWall = r->map_y + 0.5;

// 	// Compute distance to floor surface
// 	double distWall = r->perp_wall_dist;
// 	double currentDist, weight, currentFloorX, currentFloorY;

// 	for (int y = start.y; y >= end.y; y--)
// 	{
// 		currentDist = (double)WIND_HEIGHT / (2.0 * (y - WIND_HEIGHT / 2));
// 		weight = currentDist / distWall;

// 		currentFloorX = weight * (floorXWall - r->pos_x) + r->pos_x;
// 		currentFloorY = weight * (floorYWall - r->pos_y) + r->pos_y;

// 		int floorTexX = ((int)(currentFloorX * tex_w)) & (tex_w - 1);
// 		int floorTexY = ((int)(currentFloorY * tex_h)) & (tex_h - 1);

// 		if (floorTexX < 0 || floorTexX >= tex_w || floorTexY < 0 || floorTexY >= tex_h)
// 			continue;

// 		Uint32 floor_color = floor_pixels[tex_w * floorTexY + floorTexX];
// 		floor_color = (floor_color >> 1) & 8355711;

// 		SDL_SetRenderDrawColor(game->renderer,
// 			(floor_color >> 16) & 0xFF,
// 			(floor_color >> 8) & 0xFF,
// 			floor_color & 0xFF,
// 			255);
// 		SDL_RenderDrawPoint(game->renderer, start.x, y);
// 	}
// }

static void	draw_wall(t_game *game, t_raycaster *r)
{
	int			tex_x;
	int			start;
	int			line_height;
	double		wall_x;
	SDL_Rect	dest;
	SDL_Rect	src;
	int			wind_height;
	int			tex_w;
	int			tex_h;
	double		inv_perp_wall_dist;
	SDL_Texture	*wall_texture;
	SDL_SetTextureColorMod(game->textures.wall.texture, 255, 255, 255); // Reset brightness

	wind_height = WIND_HEIGHT;
	tex_w = game->textures.wall.width;
	tex_h = game->textures.wall.height;
	inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
	line_height = (int)(wind_height * inv_perp_wall_dist);
	start = ((wind_height - line_height) >> 1) + CAM_SHIFT;
	wall_x = (r->side == 0)
		? (r->pos_y + r->perp_wall_dist * r->ray_dir_y)
		: (r->pos_x + r->perp_wall_dist * r->ray_dir_x);
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * tex_w);
	tex_x &= (tex_w - 1);

	// Darken walls that face north/south (side 1)
	if (r->side == 1)
		SDL_SetTextureColorMod(game->textures.wall.texture, 180, 180, 180); // 70% brightness

	src.x = tex_x;
	src.y = 0;
	src.w = 1;
	src.h = tex_h;
	dest.x = r->x;
	dest.y = start;
	dest.h = line_height;
	dest.w = PIXEL_BLOCK;

	// Render the wall with shading
	SDL_RenderCopy(RENDERER, game->textures.wall.texture, &src, &dest);

	// Reset texture brightness for next frame
	SDL_SetTextureColorMod(game->textures.wall.texture, 255, 255, 255);
}

void	draw_floor_tile(t_game *game, t_floor_ceiling *f)
{
	int		px, py, cell_x, cell_y;
	int		floor_tx, floor_ty, ceiling_tx, ceiling_ty;
	Uint32	floor_color, ceiling_color;
	float	floor_x, floor_y, ceiling_x, ceiling_y;
	float	step_x, step_y, row_distance, pos_z;
	int		y, x, p;

	pos_z = 0.4 * WIND_HEIGHT;

	/*** Draw Floor ***/
	for (y = f->horizon; y < WIND_HEIGHT; y += PIXEL_BLOCK)
	{
		p = y - (WIND_HEIGHT / 2) - CAM_SHIFT;
		row_distance = (pos_z / p) * 2;
		step_x = row_distance * (f->ray_dir_x_1 - f->ray_dir_x_0) / WIND_WIDTH;
		step_y = row_distance * (f->ray_dir_y_1 - f->ray_dir_y_0) / WIND_WIDTH;
		floor_x = PLAYER_X + row_distance * f->ray_dir_x_0;
		floor_y = PLAYER_Y + row_distance * f->ray_dir_y_0;

		for (x = 0; x < WIND_WIDTH; x += PIXEL_BLOCK)
		{
			cell_x = (int)floor_x;
			cell_y = (int)floor_y;

			// **Bounds Check**
			if (cell_x >= 0 && cell_y >= 0 && cell_x < MAP_WIDTH && cell_y < MAP_HEIGHT && MAPS[LEVEL][cell_y][cell_x] == '0')
			{
				if (f->floor_pixels)
				{
					floor_tx = ((int)(game->textures.floor.width * (floor_x - cell_x)))
						& (game->textures.floor.width - 1);
					floor_ty = ((int)(game->textures.floor.height * (floor_y - cell_y)))
						& (game->textures.floor.height - 1);
					floor_color = f->floor_pixels[game->textures.floor.width * floor_ty + floor_tx];
					floor_color = (floor_color >> 1) & 8355711;

					// **Draw the floor block**
					for (int dy = 0; dy < PIXEL_BLOCK; dy++)
					{
						for (int dx = 0; dx < PIXEL_BLOCK; dx++)
						{
							px = x + dx;
							py = y + dy;
							if (px < WIND_WIDTH && py < WIND_HEIGHT)
								f->pixels[py * f->pitch + px] = floor_color;
						}
					}
				}
			}

			// **Advance floor position**
			floor_x += step_x * PIXEL_BLOCK;
			floor_y += step_y * PIXEL_BLOCK;
		}
	}
}

void	cast_floor_and_ceiling(t_game *game, t_floor_ceiling *f)
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
	// Compute base ray directions
	f->ray_dir_x_0 = PLAYER_DIR_X - PLAYER_CAM_X;
	f->ray_dir_y_0 = PLAYER_DIR_Y - PLAYER_CAM_Y;
	f->ray_dir_x_1 = PLAYER_DIR_X + PLAYER_CAM_X;
	f->ray_dir_y_1 = PLAYER_DIR_Y + PLAYER_CAM_Y;
	pos_z = 0.5 * WIND_HEIGHT;

	/*** Draw Ceiling ***/
	for (y = 0; y < f->horizon; y += PIXEL_BLOCK)
	{
		p = (WIND_HEIGHT / 2) - y + CAM_SHIFT;
		row_distance = (pos_z / p) * 2;
		step_x = row_distance * (f->ray_dir_x_1 - f->ray_dir_x_0) / WIND_WIDTH;
		step_y = row_distance * (f->ray_dir_y_1 - f->ray_dir_y_0) / WIND_WIDTH;
		ceiling_x = PLAYER_X + row_distance * f->ray_dir_x_0;
		ceiling_y = PLAYER_Y + row_distance * f->ray_dir_y_0;

		for (x = 0; x < WIND_WIDTH; x += PIXEL_BLOCK)
		{
			cell_x = (int)ceiling_x;
			cell_y = (int)ceiling_y;

			// **Bounds Check**
			if (cell_x >= 0 && cell_y >= 0 && cell_x < MAP_WIDTH && cell_y < MAP_HEIGHT && MAPS[LEVEL][cell_y][cell_x] == EMPTY)
			{
				if (f->ceiling_pixels)
				{
					ceiling_tx = ((int)(game->textures.ceiling.width * (ceiling_x - cell_x)))
						& (game->textures.ceiling.width - 1);
					ceiling_ty = ((int)(game->textures.ceiling.height * (ceiling_y - cell_y)))
						& (game->textures.ceiling.height - 1);
						ceiling_color = f->ceiling_pixels[game->textures.ceiling.width * ceiling_ty + ceiling_tx];
						ceiling_color = (ceiling_color >> 1) & 8355711;

					// **Draw the ceiling block**
					for (int dy = 0; dy < PIXEL_BLOCK; dy++)
					{
						for (int dx = 0; dx < PIXEL_BLOCK; dx++)
						{
							px = x + dx;
							py = y + dy;
							if (px < WIND_WIDTH && py >= 0 && py < WIND_HEIGHT)
								f->pixels[py * f->pitch + px] = ceiling_color;
						}
					}
				}
			}

			// **Advance ceiling position**
			ceiling_x += step_x * PIXEL_BLOCK;
			ceiling_y += step_y * PIXEL_BLOCK;
		}
	}

	/*** Draw Floor ***/
	for (y = f->horizon; y < WIND_HEIGHT; y += PIXEL_BLOCK)
	{
		p = y - (WIND_HEIGHT / 2) - CAM_SHIFT;
		row_distance = (pos_z / p) * 2;
		step_x = row_distance * (f->ray_dir_x_1 - f->ray_dir_x_0) / WIND_WIDTH;
		step_y = row_distance * (f->ray_dir_y_1 - f->ray_dir_y_0) / WIND_WIDTH;
		floor_x = PLAYER_X + row_distance * f->ray_dir_x_0;
		floor_y = PLAYER_Y + row_distance * f->ray_dir_y_0;

		for (x = 0; x < WIND_WIDTH; x += PIXEL_BLOCK)
		{
			cell_x = (int)floor_x;
			cell_y = (int)floor_y;

			// **Bounds Check**
			if (cell_x >= 0 && cell_y >= 0 && cell_x < MAP_WIDTH && cell_y < MAP_HEIGHT && MAPS[LEVEL][cell_y][cell_x] == EMPTY)
			{
				if (f->floor_pixels)
				{
					floor_tx = ((int)(game->textures.floor.width * (floor_x - cell_x)))
						& (game->textures.floor.width - 1);
					floor_ty = ((int)(game->textures.floor.height * (floor_y - cell_y)))
						& (game->textures.floor.height - 1);
					floor_color = f->floor_pixels[game->textures.floor.width * floor_ty + floor_tx];
					floor_color = (floor_color >> 1) & 8355711;

					// **Draw the floor block**
					for (int dy = 0; dy < PIXEL_BLOCK; dy++)
					{
						for (int dx = 0; dx < PIXEL_BLOCK; dx++)
						{
							px = x + dx;
							py = y + dy;
							if (px < WIND_WIDTH && py < WIND_HEIGHT)
								f->pixels[py * f->pitch + px] = floor_color;
						}
					}
				}
			}

			// **Advance floor position**
			floor_x += step_x * PIXEL_BLOCK;
			floor_y += step_y * PIXEL_BLOCK;
		}
	}
}


void	render_mini_rays(t_game *game, t_mini_ray_node **head, t_floor_ceiling *f)
{
	t_mini_ray_node *current = *head;
	t_mini_ray_node *temp;
	// int				x;
	// int				y;

	// x = current->ray.map_x;
	// y = current->ray.map_y;
	while (current)
	{
		
		if (current->ray.detected < 5)
		{
			half_block_up(game, &current->ray);
		}
		else
			half_down_block(game, &current->ray);
		// if (x != current->ray.map_x && y != current->ray.map_y)
		// x = current->ray.map_x;
		// y = current->ray.map_y;
		temp = current;
		current = current->next;
		free(temp);
	}

	*head = NULL;
}


static void	draw_scene(t_game *game)
{
	t_raycaster			r;
	t_floor_ceiling		f;

	if (!game->textures.screen_texture)
		cleanup(game);

	// 🔴 Wrong: f.pixels should be a pointer!
	// if (SDL_LockTexture(game->textures.screen_texture, NULL, f.pixels, &f.pitch) != 0)

	// ✅ Correct: Pass the address of f.pixels
	if (SDL_LockTexture(game->textures.screen_texture, NULL, (void **)&f.pixels, &f.pitch) != 0)
		cleanup(game);

	f.pitch /= sizeof(Uint32);

	// Render floor & ceiling
	cast_floor_and_ceiling(game, &f);
	draw_floor_tile(game, &f);

	// Render walls
	SDL_UnlockTexture(game->textures.screen_texture);
	SDL_RenderCopy(game->renderer, game->textures.screen_texture, NULL, NULL);
	r.x = 0;
	while (r.x < WIND_WIDTH)
	{
		init_raycaster(&r, game);
		init_raycaster_steps(&r);
		perform_raycaster_steps(&r, game);
		draw_wall(game, &r);
		render_mini_rays(game, &r.mini_ray, &f);
		r.x += PIXEL_BLOCK;
	}
	
}


void	render_next_frame(t_game *game)
{
	SDL_RenderClear(RENDERER);
	draw_scene(game);
	draw_minimap(game);
	SDL_RenderPresent(RENDERER);
}

	