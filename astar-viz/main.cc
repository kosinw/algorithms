#include "../astar/pathfind.hh"

#include <iostream>
#include <stdint.h>

#include <SDL.h>

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Error initializing SDL" << std::endl;
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("A* Visualization by Kosi Nwabueze",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1280, 720, 0);			

	A::Pathfinder pathfinder;	

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}