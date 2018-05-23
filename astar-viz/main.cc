#include <astar/pathfind.hh>

#include <iostream>
#include <stdint.h>
#include <vector>

#include <SDL.h>

enum CellState
{
	VISITED = 1,
	SOURCE = 2,
	GOAL = 3,
	NONE = 0
};

struct Grid
{
	explicit Grid(int32_t x, int32_t y)
		: X(x),
		Y(y),
		Points(x * y, CellState::NONE)
	{
	}

	std::vector<CellState> Points;
	int32_t X;
	int32_t Y;
};

int32_t HandleEvents(SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_QUIT:
	{
		return -1;
	}
	case SDL_KEYDOWN:
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			return -1;
		}
	}
	}

	return 0;
}

void RenderGrid(SDL_Renderer *rhnd, const Grid &grid)
{
	for (size_t i = 0; i < grid.X * grid.Y; ++i)
	{
		// draw a square
	}
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Error! SDL could not initialize properly" << std::endl;

		return -1;
	}

	SDL_Window *whnd = SDL_CreateWindow("A* Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
	SDL_Renderer *rhnd = SDL_CreateRenderer(whnd, -1, SDL_RENDERER_ACCELERATED);

	SDL_Event ev;

	Grid grid(25, 25);
	A::Pathfinder pathfinder;

	pathfinder.SetGraphSize((uint32_t)grid.X, (uint32_t)grid.Y);	

	bool running = true;

	while (running)
	{
		SDL_PollEvent(&ev);

		if (HandleEvents(ev) == -1)
		{
			running = false;
		}

		RenderGrid(rhnd, grid);
	}

	SDL_DestroyRenderer(rhnd);
	SDL_DestroyWindow(whnd);
	SDL_Quit();

	return 0;
}