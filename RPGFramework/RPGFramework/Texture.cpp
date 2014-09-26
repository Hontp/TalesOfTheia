#include "Texture.h"

Texture::Texture()
{
	textureId = 0;
	nColors = 0;
	texture_Format = NULL;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureId);


}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

unsigned int Texture::LoadTexture(const char * filename, bool GenMipMap)
{
	image = IMG_Load(filename);

	if (!image)
	{
		std::cout << "could not load file " << filename << "\n";
		return -1;
	}

	// convert SDL surface to OpenGL Texture
	nColors = image->format->BytesPerPixel;
	if (nColors == 4)
	{
		if (image->format->Rmask == 0x000000ff)
			texture_Format = GL_RGBA;
		else
			texture_Format = GL_BGRA;
	}
	else
	{	
		if (nColors == 3)
		{
			if (image->format->Rmask == 0x000000ff)
				texture_Format = GL_RGB;
			else
				texture_Format = GL_BGR;

		}
		else
		{
			return 0;
		}

	}

	glBindTexture(GL_TEXTURE_2D, textureId);

	if (GenMipMap)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (GenMipMap)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, nColors, image->w, image->h, texture_Format, GL_UNSIGNED_BYTE, image->pixels);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nColors, image->w, image->h, 0, texture_Format, GL_UNSIGNED_BYTE, image->pixels);
	}

	SDL_FreeSurface(image);

	return textureId;
}