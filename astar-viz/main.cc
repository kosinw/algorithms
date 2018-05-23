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

void RenderGrid(SDL_Renderer *rhnd, const Grid &grid, const int32_t width, const int32_t height)
{	
	SDL_SetRenderDrawColor(rhnd, 0x0, 0x0, 0x0, 1);

	int32_t hSpacing = width / grid.X;
		
	for (size_t x = 0; x < grid.X; ++x) // vertical lines
	{
		SDL_RenderDrawLine(rhnd, hSpacing * (int32_t)x, 0, hSpacing * (int32_t)x, height);
	}

	int32_t vSpacing = height / grid.Y;
	for (size_t y = 0; y < grid.Y; ++y) // horizontal lines
	{
		SDL_RenderDrawLine(rhnd, 0, vSpacing * (int32_t)y, width, vSpacing * (int32_t)y);
	}

	for (size_t i = 0; i < grid.X * grid.Y; ++i) // potential squares
	{		
		const CellState &cellState = grid.Points[i];

		// Draw grid lines

		switch (cellState)
		{
		case CellState::GOAL: // draw red square
			break;
		case CellState::NONE: // don't draw anything else
			break;
		case CellState::SOURCE: // draw green square
			break;
		case CellState::VISITED: // draw orange square
			break;
		}
	}

	SDL_SetRenderDrawColor(rhnd, 0xFF, 0xFF, 0xFF, 1);
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Error! SDL could not initialize properly" << std::endl;

		return -1;
	}

	const int32_t width = 1280;
	const int32_t height = 720;

	SDL_Window *whnd = SDL_CreateWindow("A* Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
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

		RenderGrid(rhnd, grid, width, height);
		
		SDL_RenderPresent(rhnd);
		SDL_RenderClear(rhnd);
	}

	SDL_DestroyRenderer(rhnd);
	SDL_DestroyWindow(whnd);
	SDL_Quit();

	return 0;
}