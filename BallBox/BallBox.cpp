// BallBox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SDL.h>
#include <iostream>
#include "game.hpp"
using namespace std;

constexpr auto SCREEN_WIDTH = 1024;
constexpr auto SCREEN_HEIGHT = 768;

#undef main // Fix for SDL

int main(int argc, char** args)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cout << "Error while initializing SDL. Error: " << SDL_GetError() << endl;
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Ball Box", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_ShowCursor(0);

	Game g = Game(renderer);
	g.loop();
	std::cout << "Ended!\n";

	return 0;
}