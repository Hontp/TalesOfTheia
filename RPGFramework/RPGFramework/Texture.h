#ifndef TEXTURE_H
#define TEXTURE_H

#include <glew.h>
#include <freeglut.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Texture
{
private:
	unsigned int textureId;
	GLenum texture_Format;
	GLint nColors;
	SDL_Surface* image;


public:
	Texture();
	~Texture();
	unsigned int LoadTexture(const char *filename, bool GenMipMap = false);
};

#endif