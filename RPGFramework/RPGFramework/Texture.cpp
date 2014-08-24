#include "Texture.h"

Texture::Texture()
{
	bMipMapsGenerated = false;
}

/*----------------------------------------
Name: LoadTexture2D

Params: tPath - path to texture

Result: Load texture from file
------------------------------------------*/

bool Texture::LoadTexture2D(std::string tPath, bool bGeneratedMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(tPath.c_str(), 0); // Check the file signature and deduce its format

	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(tPath.c_str());

	if (fif == FIF_UNKNOWN) // If still unkown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, tPath.c_str());
	if (!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	iWidth = FreeImage_GetWidth(dib); // Get the image width and height
	iHeight = FreeImage_GetHeight(dib);
	iBPP = FreeImage_GetBPP(dib);

	// If somehow one of these failed (they shouldn't), return failure
	if (bDataPointer == NULL || iWidth == 0 || iHeight == 0)
		return false;

	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &iTexture);
	glBindTexture(GL_TEXTURE_2D, iTexture);

	int iFormat = iBPP == 24 ? GL_BGR : iBPP == 8 ? GL_LUMINANCE : 0;
	int iInternalFormat = iBPP == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, bDataPointer);

	if (bGeneratedMipMaps)glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(dib);

	glGenSamplers(1, &iSampler);

	sPath = tPath;
	bMipMapsGenerated = bGeneratedMipMaps;


	return true;
}

/*-------------------------------------------------
Name: SetFilitering

Params: tfMagnification - mag. filter, must be from
						TextureFiltering Enum
		tMinification - min. filter must be from
						TextureFiltering Enum

Result: Set magnification and minification
		texture filter

---------------------------------------------------*/

void Texture::SetFiltering(int s_tfMagnification, int s_tfMinification)
{
	// set magnification filter

	if (s_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
	{
		glSamplerParameteri(iSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (s_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
	{
		glSamplerParameteri(iSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// set minification filter
	if (s_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
	{
		glSamplerParameteri(iSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else if (s_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
	{
		glSamplerParameteri(iSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else if (s_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)

		glSamplerParameteri(iSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	else if (s_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
	{
		glSamplerParameteri(iSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}
	else if (s_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
	{
		glSamplerParameteri(iSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	tfMagnification = s_tfMagnification;
	tfMinification = s_tfMinification;
}

/*----------------------------------------------------------------
Name: BindTexture

Params: iTextureUnit - texture unit to bind texture to

Result: Binds Texture to object

------------------------------------------------------------------*/

void Texture::BindTexture(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0 + iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, iTexture);
	glBindSampler(iTextureUnit, iSampler);
}

/*-----------------------------------------------------------------
Name: ReleaseTexture

Params: none

Result: fress all memory used by texture
-------------------------------------------------------------------*/
void Texture::ReleaseTexture()
{
	glDeleteSamplers(1, &iSampler);
	glDeleteTextures(1, &iTexture);
}

int Texture::GetMagnificationFilter()
{
	return tfMagnification;
}

int Texture::GetMinificationFilter()
{
	return tfMinification;
}
