/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	proto_3d_render(t_game *game)
{
	int line_height, start, end;
	int tex_x, tex_w, tex_h;
	double wall_x;
	SDL_Rect src, dest;

	// Correct perpendicular distance for projection
	line_height = game->wind_height / RAY.perp_wall_dist;
	start = (game->wind_height - line_height) / 2;
	end = start + line_height;

	// Get texture properties
	SDL_QueryTexture(game->textures.wall.texture, NULL, NULL, &tex_w, &tex_h);

	// Determine exact wall hit position for texture mapping
	if (RAY.side == 0)
		wall_x = RAY.pos_y + RAY.perp_wall_dist * RAY.ray_dir_y;
	else
		wall_x = RAY.pos_x + RAY.perp_wall_dist * RAY.ray_dir_x;
	wall_x -= floor(wall_x); // Get the fractional part (0-1) for texture mapping

	// Calculate the corresponding texture X coordinate
	tex_x = (int)(wall_x * tex_w);
	if (tex_x < 0) tex_x = 0;
	if (tex_x >= tex_w) tex_x = tex_w - 1;

	// Render ceiling
	draw_column(RENDERER, RAY.x, 0, start, int_to_color(0x808080FF));

	// Set texture source (scaled strip)
	src.x = tex_x;
	src.y = 0;
	src.w = 1;
	src.h = tex_h;

	// Set destination rectangle (scaled to fit wall height)
	dest.x = RAY.x;
	dest.y = start;
	dest.w = 1;
	dest.h = line_height;

	// Use SDL_RenderCopyEx() for better performance
	SDL_RenderCopyEx(RENDERER, game->textures.wall.texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

	// Render floor
	draw_column(RENDERER, RAY.x, end, game->wind_height, int_to_color(0x808080FF));
}




static void	draw_scene(t_game *game)
{
	RAY.x = 0;
	while (RAY.x < WIND_WIDTH)
	{
		init_raycaster(game);
		init_raycaster_steps(game);
		perform_raycaster_steps(game);
		proto_3d_render(game);
		RAY.x++;
	}
}

void	render_next_frame(t_game *game)
{
	SDL_RenderClear(RENDERER);
	draw_scene(game);
	draw_minimap(game);
	SDL_RenderPresent(RENDERER);
}
