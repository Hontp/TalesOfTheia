#ifndef SHADER_UTIL_H
#define SHADER_UTIL_H

#include "Shader.h"

class ShaderUtil
{
public:
	void CreateShaderProgram();
	void DeleteShaderProgram();

	bool AddShaderToProgram(Shader* oShader);
	bool LinkShader();

	void UseShaderProgram();

	UINT GetShaderProgramID();

	ShaderUtil();

private:
	UINT ProgramID; // shader program ID
	bool bLinked; // whether shader was linked and is ready to use
};

#endif