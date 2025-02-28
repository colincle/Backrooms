/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

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
			if (tex_index >= 0 && tex_index < tex_w * tex_h)
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
			if (tex_index >= 0 && tex_index < tex_w * tex_h)
			{
				set_z_buffer(game, r->perp_wall_dist, pixel_index);
				pixel_data[pixel_index] = texture[tex_index];
			}
			tex_pos += tex_step;
		}
	}
}

void	draw_wall(t_game *game, t_raycaster *r, void *pixels)
{
	int			tex_x, start, line_height, texture_height, tex_w, tex_h;
	int			clip_amount, visible_height, py, pixel_index, tex_y, tex_index;
	double		wall_x, inv_perp_wall_dist, tex_step, tex_pos;
	Uint32		*pixel_data, *texture;

	texture_height = TEXTURE_HEIGHT;
	tex_w = game->textures.wall.width;
	tex_h = game->textures.wall.height;
	inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
	line_height = ((int)(texture_height * inv_perp_wall_dist) + 4);
	start = ((texture_height - line_height) >> 1) + CAM_SHIFT
		+ ((PLAYER_HEIGHT - TEXTURE_HEIGHT) / r->perp_wall_dist);
	wall_x = (r->side == 0) * (r->pos_y + r->perp_wall_dist * r->ray_dir_y)
		+ (r->side == 1) * (r->pos_x + r->perp_wall_dist * r->ray_dir_x);
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * tex_w) & (tex_w - 1);
	texture = (r->side == 1) ? game->textures.wall_dark.pixels : game->textures.wall.pixels;
	clip_amount = 0;
	if (start < 0)
	{
		clip_amount = -start;
		start = 0;
	}
	visible_height = line_height - clip_amount;
	if (visible_height <= 0)
		return ;
	tex_step = (double)tex_h / line_height;
	tex_pos = clip_amount * tex_step;
	pixel_data = (Uint32 *)pixels;
	py = -1;
	while (++py < visible_height)
	{
		if (start + py >= 0 && start + py < texture_height)
		{
			pixel_index = (start + py) * TEXTURE_WIDTH + r->x;
			tex_y = (int)tex_pos & (tex_h - 1);
			tex_index = (tex_y * tex_w) + tex_x;
			if (tex_index >= 0 && tex_index < tex_w * tex_h)
			{
				set_z_buffer(game, r->perp_wall_dist, pixel_index);
				pixel_data[pixel_index] = texture[tex_index];
			}
		}
		tex_pos += tex_step;
	}
}
