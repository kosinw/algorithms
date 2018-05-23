#include "../astar/pathfind.hh"

#include <iostream>
#include <stdint.h>

#include <SDL.h>

#include "state.hh"

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

	return 0;
}