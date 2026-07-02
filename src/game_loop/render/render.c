#include <SDLRaycaster.h>

static void	render_mini_rays(t_game *game, t_mini_ray **head, void *pixels)
{
	t_mini_ray	*current = *head;
	t_mini_ray	*temp;

	while (current)
	{
		if (current->ray.detected >= 0 && current->ray.detected < 5)
		{
			half_block_up(game, &current->ray, pixels);
		}
		else if (current->ray.detected >= 5 && current->ray.detected < 10)
			half_down_block(game, &current->ray, pixels);
		else if (current->ray.detected == 10 && current->ray.first_hole)
			hole_block(game, &current->ray, pixels);
		else if (current->ray.detected == 11 && current->ray.first_hole)
			pillar_block(game, &current->ray, pixels);
		temp = current;
		current = current->next;
		free(temp);
	}
	*head = NULL;
}

static int	rendering_threads(void *arg)
{
	t_rendering_threads	*thread = arg;
	t_raycaster			r;
	t_floor_ceiling		f;
	int					end_x;
	f.pixels = thread->game->screen;
	r.x = (thread->TEXTURE_WIDTH * thread->thread_id) / thread->total_threads;
	end_x = (thread->TEXTURE_WIDTH * (thread->thread_id + 1)) / thread->total_threads;
	thread->start = r.x;
	thread->end = end_x;
	while (r.x < end_x)
	{
		init_raycaster(&r, thread->game);
		init_raycaster_steps(&r);
		perform_raycaster_steps(&r, thread->game);
		draw_wall(thread->game, &r, thread->game->screen);
		render_mini_rays(thread->game, &r.mini_ray, f.pixels);
		r.x++;
	}
	cast_floor_and_ceiling(thread->game, &f, thread);
	cast_offset_height_floor_and_ceiling(thread->game, &f, thread);
	return (0);
}

static void	draw_scene(t_game *game)
{
	int					pitch;
	Uint32				*pixels;
	t_rendering_threads	thread[game->P_cores];
	SDL_Thread			*threads[game->P_cores];

	for (int i = 0; i < game->P_cores; i++)
	{
		thread[i].game = game;
		thread[i].thread_id = i;
		thread[i].total_threads = game->P_cores;
		thread[i].start = (TEXTURE_WIDTH * i) / game->P_cores;
		thread[i].end = (TEXTURE_WIDTH * (i + 1)) / game->P_cores;
		threads[i] = SDL_CreateThread(rendering_threads, "render", &thread[i]);
	}
	for (int i = 0; i < game->P_cores; i++)
	{
		SDL_WaitThread(threads[i], NULL);
	}
	if (!game->textures.screen_texture)
		cleanup(game);
	if (SDL_LockTexture(game->textures.screen_texture, NULL, (void **)&pixels, &pitch) != 0)
		cleanup(game);
	memcpy(pixels, game->screen, game->texture_height * pitch);
	SDL_UnlockTexture(game->textures.screen_texture);
	SDL_RenderCopy(game->renderer, game->textures.screen_texture, NULL, NULL);
	clear_z_buffer(game);
}

void	render_next_frame(t_game *game)
{
	SDL_RenderClear(RENDERER);
	draw_scene(game);
	draw_minimap(game);
	SDL_RenderPresent(RENDERER);
}
