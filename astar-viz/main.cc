#include "astar/min_heap.hh"

#include <iostream>
#include <stdint.h>
#include <vector>

#include <SDL.h>

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

	bool running = true;

	while (running)
	{
		SDL_PollEvent(&ev);

		if (ev.type == SDL_KEYDOWN)
		{
			if (ev.key.keysym.sym == SDLK_ESCAPE)
			{
				running = false;
			}
		}

		SDL_RenderPresent(rhnd);
		SDL_RenderClear(rhnd);
	}

	SDL_DestroyRenderer(rhnd);
	SDL_DestroyWindow(whnd);
	SDL_Quit();

	return 0;
}