#include "Shader.h"

Shader::Shader()
{
	bLoaded = false;
}

/*---------------------------------------------

Name:	loadShader

Params : sFile - path to a file
			 s_iType - type of shader(fragment, vertex, geometry)

Result : Loads and compiles shader
---------------------------------------------*/

bool Shader::LoadShader(std::string sFile, int s_iType)
{
	FILE* fp;
	fopen_s(&fp, sFile.c_str(), "rt");
	if (!fp)
		return false;

	// get all lines from a file
	std::vector<std::string> sLines;
	char sLine[255];

	while (fgets(sLine, 255, fp))
	{
		sLines.push_back(sLine);
	}
	fclose(fp);

	const char** sProgram = new const char*[sLines.size()];
	for (unsigned int i = 0; i < sLines.size(); i++)
	{
		sProgram[i] = sLines[i].c_str();
	}

	idShader = glCreateShader(s_iType);

	glShaderSource(idShader, sLines.size(), sProgram, NULL);
	glCompileShader(idShader);

	delete[] sProgram;

	int iCompilationStatus;
	glGetShaderiv(idShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if (iCompilationStatus == GL_COMPILE_STATUS)
		return false;

	iType = s_iType;
	bLoaded = true;

	return true;
	
}

/*----------------------------------------------

Name:	IsLoaded

Params:	none

Result:	True if shader was loaded and compiled.

---------------------------------------------*/

bool Shader::IsLoaded()
{
	return bLoaded;
}

/*-----------------------------------------------

Name:	GetShaderID

Params:	none

Result:	Returns ID of a genereated shader

---------------------------------------------*/
UINT Shader::GetShaderID()
{
	return idShader;
}

/*-----------------------------------------------

Name:	DeleteShader

Params:	none

Result:	Deletes shader and free GPU memory

/*---------------------------------------------*/

void Shader::DeleteShader()
{
	if (!IsLoaded())
		return;

	bLoaded = false;

	glDeleteShader(idShader);
}