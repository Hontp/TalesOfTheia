#ifndef SHADER_H
#define SHADER_H

#include<glew.h>
#include<fstream>
#include<string>
#include<iostream>
#include<vector>

class Shader
{
private:
	unsigned id;
	unsigned int vertShader;
	unsigned int fragShader;
	unsigned int program;
	unsigned int LoadShader(std::string& source, unsigned int mode);
	void LoadFile(const char* filename, std::string& stream);
	
public:
	Shader(const char* vShader, const char* fShader);
	~Shader();

	unsigned int GetProgramID();
	void UseShader();
	void DeleteShader();
};

#endif