// SDLTesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char *argv[])
{
	// Initialisies SDL Video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// Displays error message
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_INIT failed", NULL);
		return 1;
	}

	// Creates a window using pointer
	SDL_Window* window = SDL_CreateWindow(
		"My first window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_BORDERLESS
	);

	// Checks in window has been created
	if (window == nullptr)
	{
		// Displays error message
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_CreateWindow failed", NULL);
		// Closes the SDL libary
		SDL_Quit();
		return 1;
	}

	// Main loop
	bool running = true;
	// Creates SDL event structure
	SDL_Event ev;

	while (running)
	{
		// Poll for events and fills event structure
		while (SDL_PollEvent(&ev))
		{
			// Switch case only for events we are interested in
			switch (ev.type)
			{
			// When window has been clsoed
			case SDL_QUIT:
				running = false;
				break;
			// When key has been pressed
			case SDL_KEYDOWN:
				// Checks the actual key code of key that has been pressed
				switch (ev.key.keysym.sym)
				{
					// Escape key
				case SDLK_ESCAPE:
					running = false;
					break;

				}
			}
		}
		// Gets the window surface
		SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
		// Fills window with colour
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 255, 0));
		// Updates the surface
		SDL_UpdateWindowSurface(window);
	}
	// Destroys the window
	SDL_DestroyWindow(window);
	// Quits SDL2
	SDL_Quit();

    return 0;
}

