//int first
//one line to load an image
//error checking
//nullptr
//gen -> bind
//state machine, once bound every operation will be done on the bound object
//FOR EVERY GEN THERE IS A DELETE
//cgtextures.com
//opengamemap
//exercise on slides

#include "Texture.h"

GLuint loadTextureFromFile(const std::string& filename)
{
	GLuint textureID;

	SDL_Surface * surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
	{
		printf("Could not load file %s", IMG_GetError());
			return 0;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

	// No longer need the surface
	SDL_FreeSurface(surface);

	return textureID;
}