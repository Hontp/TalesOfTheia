#ifndef SHADER_H
#define SHADER_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib,"glew32.lib")

#include <glew.h>
#include<string>
#include<cstdio>
#include<vector>

typedef unsigned int UINT;

class Shader
{
public:
	bool LoadShader(std::string sFile, int iType);
	void DeleteShader();

	bool IsLoaded();
	UINT GetShaderID();

	Shader();

private:

	UINT idShader; // ID of shader
	int iType;	// type of shaders
	bool bLoaded; // wheater shaders was loaded and compiled

};

#endif