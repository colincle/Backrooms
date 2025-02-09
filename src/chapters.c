/*
** SDLRaycaster - a from-scratch raycasting engine in C with SDL2
** Author: Clement Colin
*/

#include <SDLRaycaster.h>

static void	first_time_chapter_called(t_game *game, int *first, int *running)
{
	(void)game;
	(void)*running;
	*first = FALSE;
}

void	chapter_1(t_game *game, int *running)
{
	static int	first = TRUE;
	int			chapter_end;

	chapter_end = FALSE;
	if (first)
		first_time_chapter_called(game, &first, running);
	if (chapter_end)
		LEVEL++;
}

void	chapter_2(t_game *game, int *running)
{
	static int	first = TRUE;
	int			chapter_end;

	chapter_end = FALSE;
	first_time_chapter_called(game, &first, running);
	if (chapter_end)
		LEVEL++;
}

void	chapter_3(t_game *game, int *running)
{
	static int	first = TRUE;
	int			chapter_end;

	chapter_end = FALSE;
	first_time_chapter_called(game, &first, running);
	if (chapter_end)
		LEVEL++;
}

void	chapter_4(t_game *game, int *running)
{
	static int	first = TRUE;
	int			chapter_end;

	chapter_end = FALSE;
	first_time_chapter_called(game, &first, running);
	if (chapter_end)
		LEVEL++;
}

void	chapter_5(t_game *game, int *running)
{
	static int	first = TRUE;
	int			chapter_end;

	chapter_end = FALSE;
	first_time_chapter_called(game, &first, running);
	if (chapter_end)
		*running = FALSE;
}
