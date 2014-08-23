#ifndef BIND_SHADER_H
#define BIND_SHADER_H

#include "Shader.h"

class BindShader
{
public:
	void CreateShader();
	void DeleteShader();

	bool AddShader(Shader* oShader);
	bool LinkShader();

	void ActivateShader();

	UINT GetBindedShaderID();

	BindShader();

private:
	UINT idBindedProgram; // ID of shader
	bool bLinked; // whether shader was linked and is ready to use
};

#endif