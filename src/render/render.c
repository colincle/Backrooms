/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static inline void proto_3d_render(t_game *game, t_raycaster *r)
{
    int line_height, start, tex_x;
    double wall_x;
    SDL_Rect src, dest;

    // **Precompute common values**
    int wind_height = WIND_HEIGHT;
    int tex_w = game->textures.wall.width;
    int tex_h = game->textures.wall.height;

    // **Direct division instead of fast_div()**
    double inv_perp_wall_dist = 1.0 / (r->perp_wall_dist / 2);
    // double inv_perp_wall_dist = 1.0 / (r->perp_wall_dist);

    // **Compute projected line height**
    line_height = (int)(wind_height * inv_perp_wall_dist);
    start = (wind_height - line_height) >> 1;  // Use bit shift instead of / 2

    // **Compute exact wall hit position**
    wall_x = (r->side == 0) 
        ? (r->pos_y + r->perp_wall_dist * r->ray_dir_y)
        : (r->pos_x + r->perp_wall_dist * r->ray_dir_x);
    
    wall_x -= (int)wall_x;  // Faster than fmod()

    // **Compute texture X coordinate (Optimized)**
    tex_x = (int)(wall_x * tex_w);
    tex_x &= (tex_w - 1); // Use bitwise AND if width is power of 2


    // **Set texture source & destination**
    src.x = tex_x;
    src.y = 0;
    src.w = 1;
    src.h = tex_h;

    dest.x = r->x;
    dest.y = start;
    dest.w = PIXEL_BLOCK; // **Macro for block width**
    dest.h = line_height;

    // **Batch Rendering: Reduce GPU Calls**
    SDL_RenderCopy(RENDERER, game->textures.wall.texture, &src, &dest);
}

void cast_floor(t_game *game)
{
	Uint32 *pixels;
	int pitch;

	if (!game->textures.screen_texture)
	{
		printf("Error: screen_texture is NULL\n");
		return;
	}

	if (SDL_LockTexture(game->textures.screen_texture, NULL, (void **)&pixels, &pitch) != 0)
	{
		printf("SDL_LockTexture failed: %s\n", SDL_GetError());
		return;
	}

	pitch /= sizeof(Uint32);

	// **Precompute constants (done once per frame)**
	float ray_dir_x_0 = PLAYER_DIR_X - PLAYER_CAM_X;
	float ray_dir_y_0 = PLAYER_DIR_Y - PLAYER_CAM_Y;
	float ray_dir_x_1 = PLAYER_DIR_X + PLAYER_CAM_X;
	float ray_dir_y_1 = PLAYER_DIR_Y + PLAYER_CAM_Y;
	float pos_z = 0.5 * WIND_HEIGHT;

	// **Process floor in steps of PIXEL_BLOCK**
	for (int y = WIND_HEIGHT / 2 + 1; y < WIND_HEIGHT; y += PIXEL_BLOCK)
	{
		int p = y - WIND_HEIGHT / 2;
		float row_distance = (pos_z / p) * 2;

		float step_x = row_distance * (ray_dir_x_1 - ray_dir_x_0) / WIND_WIDTH;
		float step_y = row_distance * (ray_dir_y_1 - ray_dir_y_0) / WIND_WIDTH;

		float floor_x = PLAYER_X + row_distance * ray_dir_x_0;
		float floor_y = PLAYER_Y + row_distance * ray_dir_y_0;
		float ceiling_x = floor_x;
		float ceiling_y = floor_y;

		for (int x = 0; x < WIND_WIDTH; x += PIXEL_BLOCK)
		{
			// **Floor texture lookup**
			int cell_x = (int)floor_x;
			int cell_y = (int)floor_y;
			int floor_tx = (int)(game->textures.floor.width * (floor_x - cell_x)) & (game->textures.floor.width - 1);
			int floor_ty = (int)(game->textures.floor.height * (floor_y - cell_y)) & (game->textures.floor.height - 1);

			// **Ceiling texture lookup**
			int ceil_cell_x = (int)ceiling_x;
			int ceil_cell_y = (int)ceiling_y;
			int ceiling_tx = (int)(game->textures.ceiling.width * (ceiling_x - ceil_cell_x)) & (game->textures.ceiling.width - 1);
			int ceiling_ty = (int)(game->textures.ceiling.height * (ceiling_y - ceil_cell_y)) & (game->textures.ceiling.height - 1);

			// **Move forward in the world**
			floor_x += step_x * PIXEL_BLOCK;
			floor_y += step_y * PIXEL_BLOCK;
			ceiling_x += step_x * PIXEL_BLOCK;
			ceiling_y += step_y * PIXEL_BLOCK;

			// **Correct texture pointers**
			Uint32 *floor_pixels = game->textures.floor.pixels;
			Uint32 *ceiling_pixels = game->textures.ceiling.pixels;

			if (!floor_pixels || !ceiling_pixels)
			{
				printf("Error: floor or ceiling pixels are NULL\n");
				SDL_UnlockTexture(game->textures.screen_texture);
				return;
			}

			// **Fetch correct pixels**
			Uint32 floor_color = floor_pixels[game->textures.floor.width * floor_ty + floor_tx];
			Uint32 ceiling_color = ceiling_pixels[game->textures.ceiling.width * ceiling_ty + ceiling_tx];

			// **Darken for lighting effect**
			floor_color = (floor_color >> 1) & 8355711;
			ceiling_color = (ceiling_color >> 1) & 8355711;

			// **Fill PIXEL_BLOCK × PIXEL_BLOCK pixels**
			for (int dy = 0; dy < PIXEL_BLOCK; dy++)
			{
				for (int dx = 0; dx < PIXEL_BLOCK; dx++)
				{
					int px = x + dx;
					int py = y + dy;
					if (px < WIND_WIDTH && py < WIND_HEIGHT)
					{
						pixels[(WIND_HEIGHT - py - 1) * pitch + px] = ceiling_color; // Ceiling FIRST
						pixels[py * pitch + px] = floor_color;  // Floor SECOND
					}
				}
			}
		}
	}

	SDL_UnlockTexture(game->textures.screen_texture);
	SDL_RenderCopy(game->renderer, game->textures.screen_texture, NULL, NULL);
}




static void draw_scene(t_game *game)
{
	t_raycaster r;

	// **Render floor & ceiling before walls**
	cast_floor(game);

	r.x = 0;
	while (r.x < WIND_WIDTH)
	{
		init_raycaster(&r, game);
		init_raycaster_steps(&r);
		perform_raycaster_steps(&r, game);
		proto_3d_render(game, &r);
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
