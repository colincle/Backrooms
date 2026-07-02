#include <SDLRaycaster.h>

void	half_down_block(t_game *game, t_raycaster *r, void *pixels)
{
	int			tex_x, start, block_height, texture_height, tex_w, tex_h;
	int			pixel_index, tex_y, tex_index, py, clip_amount, visible_height;
	int			line_height, y;
	double		wall_x, inv_perp_wall_dist, tex_step, tex_pos, normalized;
	Uint32		*pixel_data, *texture;
	SDL_Rect	dest;

	texture_height = TEXTURE_HEIGHT;
	tex_w = game->textures.wall.width;
	tex_h = game->textures.wall.height;
	inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
	line_height = (int)(texture_height * inv_perp_wall_dist);
	start = ((texture_height - line_height) >> 1) + CAM_SHIFT
		+ ((PLAYER_HEIGHT - TEXTURE_HEIGHT) / r->perp_wall_dist);
	normalized = (r->detected - 5) / 4.0;
	block_height = (int)(line_height * (0.1 + (normalized * 0.8)));
	wall_x = (r->side == 0)
		? (r->pos_y + r->perp_wall_dist * r->ray_dir_y)
		: (r->pos_x + r->perp_wall_dist * r->ray_dir_x);
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * tex_w) & (tex_w - 1);
	texture = (r->side == 1) ? game->textures.wall_dark.pixels : game->textures.wall.pixels;
	dest = (SDL_Rect){r->x, start, 1, block_height};
	clip_amount = 0;
	if (dest.y < 0)
	{
		clip_amount = -dest.y;
		dest.y = 0;
	}
	visible_height = block_height - clip_amount;
	if (visible_height <= 0)
		return ;
	tex_step = (double)tex_h / line_height;
	tex_pos = clip_amount * tex_step;
	pixel_data = (Uint32 *)pixels;
	y = -1;
	while (++y < visible_height)
	{
		py = dest.y + y;
		if (py >= 0 && py < texture_height)
		{
			pixel_index = py * TEXTURE_WIDTH + dest.x;
			tex_y = (int)tex_pos & (tex_h - 1);
			tex_index = (tex_y * tex_w) + tex_x;
			if (tex_index >= 0 && tex_index < tex_w * tex_h && !(pixel_index < 0 || pixel_index >= TEXTURE_WIDTH * TEXTURE_HEIGHT))
			{
				set_z_buffer(game, r->perp_wall_dist, pixel_index);
				pixel_data[pixel_index] = texture[tex_index];
			}
			tex_pos += tex_step;
		}
	}
}

void	half_block_up(t_game *game, t_raycaster *r, void *pixels)
{
	int			tex_x, tex_y, start, block_height, texture_height, tex_w, tex_h;
	int			py;
	int			pixel_index;
	int			tex_index;
	int			visible_height;
	int			clip_amount;
	int			line_height;
	double		wall_x, inv_perp_wall_dist;
	double		tex_step;
	double		tex_pos;
	double		normalized;
	Uint32		*pixel_data = (Uint32 *)pixels;
	Uint32		*texture;
	SDL_Rect	dest;

	texture_height = TEXTURE_HEIGHT;
	tex_w = game->textures.wall.width;
	tex_h = game->textures.wall.height;
	inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
	line_height = ((int)(texture_height * inv_perp_wall_dist) + 4);
	start = ((texture_height - line_height) >> 1) + CAM_SHIFT + ((PLAYER_HEIGHT - TEXTURE_HEIGHT) / r->perp_wall_dist);
	normalized = r->detected / 4.0;
	block_height = (int)(line_height * (0.1 + (normalized * 0.8)));
	wall_x = (r->side == 0)
		? (r->pos_y + r->perp_wall_dist * r->ray_dir_y)
		: (r->pos_x + r->perp_wall_dist * r->ray_dir_x);
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * tex_w) & (tex_w - 1);
	if (r->side == 1)
		texture = game->textures.wall_dark.pixels;
	else
		texture = game->textures.wall.pixels;
	dest.x = r->x;
	dest.y = start + (line_height - block_height);
	dest.w = 1;
	dest.h = block_height;
	clip_amount = 0;
	if (dest.y < 0)
	{
		clip_amount = -dest.y;
		dest.y = 0;
	}
	visible_height = block_height - clip_amount;
	if (visible_height <= 0)
		return ;
	tex_step = (double)tex_h / line_height;
	tex_pos = clip_amount * tex_step;
	for (int y = 0; y < visible_height; y++)
	{
		py = dest.y + y;
		if (py >= 0 && py < texture_height)
		{
			pixel_index = py * TEXTURE_WIDTH + dest.x;
			tex_y = (int)tex_pos & (tex_h - 1);
			tex_index = (tex_y * tex_w) + tex_x;
			if (tex_index >= 0 && tex_index < tex_w * tex_h && !(pixel_index < 0 || pixel_index >= TEXTURE_WIDTH * TEXTURE_HEIGHT))
			{
				set_z_buffer(game, r->perp_wall_dist, pixel_index);
				pixel_data[pixel_index] = texture[tex_index];
			}
			tex_pos += tex_step;
		}
	}
}

void	hole_block(t_game *game, t_raycaster *r, void *pixels)
{
	if (r->side_dist_x < r->side_dist_y)
	{
		r->side_dist_x += r->delta_dist_x;
		r->map_x += r->step_x;
		r->side = 0;
	}
	else
	{
		r->side_dist_y += r->delta_dist_y;
		r->map_y += r->step_y;
		r->side = 1;
	}

	// Recalculate wall distance after taking the extra step
	r->perp_wall_dist = (r->side == 0)
		? (r->map_x - r->pos_x + (1 - r->step_x) / 2) / r->ray_dir_x
		: (r->map_y - r->pos_y + (1 - r->step_y) / 2) / r->ray_dir_y;

	// Store perp_wall_dist to avoid modifications inside the block
	double perp_wall_dist = r->perp_wall_dist;

	{
		int			tex_x, start, line_height, tex_w, tex_h;
		int			py, pixel_index, tex_y, tex_index;
		double		wall_x, inv_perp_wall_dist, tex_step, tex_pos;
		Uint32		*pixel_data, *texture;

		tex_w = game->textures.wall.width;
		tex_h = game->textures.wall.height;
		inv_perp_wall_dist = 1.0 / (perp_wall_dist / 2);
		
		line_height = ((int)(TEXTURE_HEIGHT * inv_perp_wall_dist) + 4);

// Keep the original start point
		start = ((TEXTURE_HEIGHT - line_height) >> 1) + CAM_SHIFT
			+ ((PLAYER_HEIGHT - TEXTURE_HEIGHT) / perp_wall_dist);

		// Shift downward to correctly align the hole
		start += line_height - 2;


		wall_x = (r->side == 0) 
			? (r->pos_y + perp_wall_dist * r->ray_dir_y)
			: (r->pos_x + perp_wall_dist * r->ray_dir_x);
		wall_x -= (int)wall_x;
		tex_x = (int)(wall_x * tex_w) & (tex_w - 1);
		texture = (r->side == 1) ? game->textures.wall_dark.pixels : game->textures.wall.pixels;

		tex_step = (double)tex_h / line_height;  // Texture scaling
		tex_pos = (tex_h / 2);  // Start at the middle of the texture
		pixel_data = (Uint32 *)pixels;
		py = 0;

		// Draw only the bottom half
		while (start + py < TEXTURE_HEIGHT)
		{
			pixel_index = (start + py) * TEXTURE_WIDTH + r->x;
			tex_y = (int)tex_pos;
			tex_y = fmin(tex_y, tex_h - 1);  // Prevent overflow
			tex_index = (tex_y * tex_w) + tex_x;

			if (tex_index >= 0 && tex_index < tex_w * tex_h && !(pixel_index < 0 || pixel_index >= TEXTURE_WIDTH * TEXTURE_HEIGHT))
			{
				// Darkness depends on position in the texture, not world height
				float darkness_factor = (float)(tex_y - (tex_h / 2)) / (float)(tex_h / 2);
				darkness_factor = fmax(0.0, fmin(1.0, darkness_factor)); // Clamp 0-1

				// Get the original texture color
				Uint32 color = texture[tex_index];

				// Extract RGB components
				Uint8 r = (color >> 16) & 0xFF;
				Uint8 g = (color >> 8) & 0xFF;
				Uint8 b = color & 0xFF;

				// Apply smooth gradient darkness
				r *= (1.0 - darkness_factor);
				g *= (1.0 - darkness_factor);
				b *= (1.0 - darkness_factor);

				// Combine back into color
				Uint32 darkened_color = (r << 16) | (g << 8) | b;
				set_z_buffer(game, perp_wall_dist, pixel_index);
				pixel_data[pixel_index] = darkened_color;
			}

			tex_pos += tex_step;
			py++;
		}
	}
}
void	pillar_block(t_game *game, t_raycaster *r, void *pixels)
{
	int			tex_x, start, line_height, tex_w, tex_h;
	int			perp_wall_dist;
	int			py, pixel_index, tex_y, tex_index;
	double		wall_x, inv_perp_wall_dist, tex_step, tex_pos;
	Uint32		*pixel_data, *texture;

	perp_wall_dist = r->perp_wall_dist;
	tex_w = game->textures.wall.width;
	tex_h = game->textures.wall.height;
	inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
	
	line_height = ((int)(TEXTURE_HEIGHT * inv_perp_wall_dist) + 4);
// Keep the original start point
	start = ((TEXTURE_HEIGHT - line_height) >> 1) + CAM_SHIFT
		+ ((PLAYER_HEIGHT - TEXTURE_HEIGHT) / perp_wall_dist);
	// Shift downward to correctly align the hole
	start += line_height - 2;
	wall_x = (r->side == 0) 
		? (r->pos_y + perp_wall_dist * r->ray_dir_y)
		: (r->pos_x + perp_wall_dist * r->ray_dir_x);
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * tex_w) & (tex_w - 1);
	texture = (r->side == 1) ? game->textures.wall_dark.pixels : game->textures.wall.pixels;
	tex_step = (double)tex_h / line_height;  // Texture scaling
	tex_pos = (tex_h / 2);  // Start at the middle of the texture
	pixel_data = (Uint32 *)pixels;
	py = 0;
	// Draw only the bottom half
	while (start + py < TEXTURE_HEIGHT)
	{
		pixel_index = (start + py) * TEXTURE_WIDTH + r->x;
		tex_y = (int)tex_pos;
		tex_y = fmin(tex_y, tex_h - 1);  // Prevent overflow
		tex_index = (tex_y * tex_w) + tex_x;
		if (tex_index >= 0 && tex_index < tex_w * tex_h && !(pixel_index < 0 || pixel_index >= TEXTURE_WIDTH * TEXTURE_HEIGHT))
		{
			// Darkness depends on position in the texture, not world height
			float darkness_factor = (float)(tex_y - (tex_h / 2)) / (float)(tex_h / 2);
			darkness_factor = fmax(0.0, fmin(1.0, darkness_factor)); // Clamp 0-1
			// Get the original texture color
			Uint32 color = texture[tex_index];
			// Extract RGB components
			Uint8 r = (color >> 16) & 0xFF;
			Uint8 g = (color >> 8) & 0xFF;
			Uint8 b = color & 0xFF;
			// Apply smooth gradient darkness
			r *= (1.0 - darkness_factor);
			g *= (1.0 - darkness_factor);
			b *= (1.0 - darkness_factor);
			// Combine back into color
			Uint32 darkened_color = (r << 16) | (g << 8) | b;
			set_z_buffer(game, perp_wall_dist, pixel_index);
			pixel_data[pixel_index] = darkened_color;
		}
		tex_pos += tex_step;
		py++;
	}
}

void	draw_wall(t_game *game, t_raycaster *r, void *pixels)
{
	int			tex_x, start, line_height, texture_height, tex_w, tex_h;
	int			clip_amount, visible_height, py, pixel_index, tex_y, tex_index;
	double		perp_wall_dist, wall_x, inv_perp_wall_dist, tex_step, tex_pos;
	Uint32		*pixel_data, *texture;

	// Use `double` for precision
	perp_wall_dist = r->perp_wall_dist;
	texture_height = TEXTURE_HEIGHT;
	tex_w = game->textures.wall.width;
	tex_h = game->textures.wall.height;
	
	// Prevent division by zero or too small values
	if (perp_wall_dist < 0.001)
		perp_wall_dist = 0.001;
	inv_perp_wall_dist = 1.0 / (perp_wall_dist / 2);

	line_height = ((int)(texture_height * inv_perp_wall_dist) + 4);
	start = ((texture_height - line_height) >> 1) + CAM_SHIFT
		+ ((PLAYER_HEIGHT - TEXTURE_HEIGHT) / perp_wall_dist);

		if (r->side == 0)
		wall_x = r->pos_y + perp_wall_dist * r->ray_dir_y;
	else
		wall_x = r->pos_x + perp_wall_dist * r->ray_dir_x;
	
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * tex_w) & (tex_w - 1);
	texture = (r->side == 1) ? game->textures.wall_dark.pixels : game->textures.wall.pixels;

	// Adjust clipping
	clip_amount = 0;
	if (start < 0)
	{
		clip_amount = -start;
		start = 0;
	}
	visible_height = line_height - clip_amount;
	if (visible_height <= 0)
		return;

	tex_step = (double)tex_h / line_height;
	tex_pos = clip_amount * tex_step;
	pixel_data = (Uint32 *)pixels;
	py = -1;

	while (++py < visible_height)
	{
		// Calculate pixel index inside the screen buffer
		pixel_index = (start + py) * TEXTURE_WIDTH + r->x;

		// Ensure pixel_index is within bounds **before** using it
		if (pixel_index >= 0 && pixel_index < TEXTURE_WIDTH * TEXTURE_HEIGHT)
		{
			tex_y = (int)tex_pos & (tex_h - 1);
			tex_index = (tex_y * tex_w) + tex_x;

			if (tex_index >= 0 && tex_index < tex_w * tex_h)
			{
				set_z_buffer(game, perp_wall_dist, pixel_index);
				pixel_data[pixel_index] = texture[tex_index];
			}
		}
		tex_pos += tex_step;
	}
}
