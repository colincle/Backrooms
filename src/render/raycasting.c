/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	analyse_ray_data(t_game *game)
{
	if (RAY.side)
	{
		if (RAY.ray_dir_y > 0)
		{
			RAY.wall_dir = SOUTH;
			return ;
		}
		RAY.wall_dir = NORTH;
		return ;
	}
	if (RAY.ray_dir_x > 0)
	{
		RAY.wall_dir = EAST;
		return ;
	}
	RAY.wall_dir = WEST;
}

static void	ray_has_hit_wall(t_game *game)
{
	double	inv_ray_dir_x;
	double	inv_ray_dir_y;
	double	perp_dist;

	inv_ray_dir_x = 1.0f / RAY.ray_dir_x;
	inv_ray_dir_y = 1.0f / RAY.ray_dir_y;
	if (RAY.side == 0)
		RAY.perp_wall_dist = (RAY.map_x - RAY.pos_x + (1 - RAY.step_x) * 0.5f) * inv_ray_dir_x;
	else
		RAY.perp_wall_dist = (RAY.map_y - RAY.pos_y + (1 - RAY.step_y) * 0.5f) * inv_ray_dir_y;
	perp_dist = RAY.perp_wall_dist;
	RAY.wall_hit_x = RAY.pos_x + RAY.ray_dir_x * perp_dist;
	RAY.wall_hit_y = RAY.pos_y + RAY.ray_dir_y * perp_dist;
}

void	perform_raycaster_steps(t_game *game)
{
	char	**map;

	map = MAPS[LEVEL];
	while (map[RAY.map_y][RAY.map_x] != WALL)
	{
		// if (map[RAY.map_y][RAY.map_x] == DOOR_CLOSED)
		// {
		// 	if (ray_has_hit_door(r, game))
		// 	{
		// 		RAY.wall_dir = DOOR;
		// 		return ;
		// 	}
		// }
		if (RAY.side_dist_x < RAY.side_dist_y)
		{
			RAY.side_dist_x += RAY.delta_dist_x;
			RAY.map_x += RAY.step_x;
			RAY.side = 0;
		}
		else
		{
			RAY.side_dist_y += RAY.delta_dist_y;
			RAY.map_y += RAY.step_y;
			RAY.side = 1;
		}
	}
	ray_has_hit_wall(game);
	analyse_ray_data(game);
}

void	init_raycaster_steps(t_game *game)
{
	if (RAY.ray_dir_x < 0)
	{
		RAY.step_x = -1;
		RAY.side_dist_x = (RAY.pos_x - RAY.map_x) * RAY.delta_dist_x;
	}
	else
	{
		RAY.step_x = 1;
		RAY.side_dist_x = (RAY.map_x + 1.0 - RAY.pos_x) * RAY.delta_dist_x;
	}
	if (RAY.ray_dir_y < 0)
	{
		RAY.step_y = -1;
		RAY.side_dist_y = (RAY.pos_y - RAY.map_y) * RAY.delta_dist_y;
		return ;
	}
	RAY.step_y = 1;
	RAY.side_dist_y = (RAY.map_y + 1.0 - RAY.pos_y) * RAY.delta_dist_y;
}

void	init_raycaster(t_game *game)
{
	RAY.pos_x = PLAYER_X;
	RAY.pos_y = PLAYER_Y;
	RAY.map_x = (int)RAY.pos_x;
	RAY.map_y = (int)RAY.pos_y;
	RAY.cam_x = 2.0f * (float)RAY.x / (float)WIND_WIDTH - 1.0f;
	RAY.ray_dir_x = PLAYER_DIR_X + PLAYER_CAM_X * RAY.cam_x;
	RAY.ray_dir_y = PLAYER_DIR_Y + PLAYER_CAM_Y * RAY.cam_x;
	if (RAY.ray_dir_x == 0.0f)
		RAY.delta_dist_x = 1e10f;
	else
		RAY.delta_dist_x = fabsf(1.0f / RAY.ray_dir_x);
	if (RAY.ray_dir_y == 0.0f)
		RAY.delta_dist_y = 1e10f;
	else
		RAY.delta_dist_y = fabsf(1.0f / RAY.ray_dir_y);
}
