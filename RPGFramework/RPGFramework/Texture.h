#ifndef TEXTURE_H
#define TEXTURE_H


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib, "FreeImage.Lib")

#include <glew.h>
#include<string>

typedef unsigned int UINT;

enum TextureFilitering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

class Texture
{
public:
	bool LoadTexture2D(std::string tPath, bool bGeneratedMipMaps = false);
	void BindTexture(int iTextureUnit = 0);

	void SetFiltering(int s_tfMagnification, int s_tfMagnifcation);

	int GetMiniFicationFilter();
	int GetMagnificationFilter();

	void ReleaseTexture();

	Texture();

private:
	int iWidth, iHeight, iBPP; // texture width, height, bytes per pixel
	UINT iTexture; // texture name
	UINT iSampler; // Sampler name

	bool bMipMapGenerated;

	int tfMagnification, tfMinification;

	std::string sPath;
	
};
#endif