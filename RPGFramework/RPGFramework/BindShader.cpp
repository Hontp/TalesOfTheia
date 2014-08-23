#include"BindShader.h"

BindShader::BindShader()
{
	bLinked = false;
}

/*-----------------------------------------------

Name:	CreateShader

Params:	none

Result:	Creates a new shader

---------------------------------------------*/

void BindShader::CreateShader()
{
	idBindedProgram = glCreateProgram();
}

/*-----------------------------------------------

Name:	AddShader

Params:	oShader - shader file

Result:	adds a shader file to program

---------------------------------------------*/

bool BindShader::AddShader(Shader* oShader)
{
	if (!oShader->IsLoaded())
		return false;

	glAttachShader(idBindedProgram, oShader->GetShaderID());

	return true;
}

/*-----------------------------------------------

Name:	LinkShader

Params:	none

Result:	link shader to OpenGL program

---------------------------------------------*/

bool BindShader::LinkShader()
{
	glLinkProgram(idBindedProgram);
	int iLinkStatus;
	glGetProgramiv(idBindedProgram, GL_LINK_STATUS, &iLinkStatus);
	if (iLinkStatus == GL_TRUE)
		bLinked = true;

	return bLinked;
}

/*-----------------------------------------------

Name:	DeleteShader

Params:	none

Result:	Deletes shader and frees GPU memory

---------------------------------------------*/

void BindShader::DeleteShader()
{
	if (!bLinked)
		return;

	bLinked = false;
	glDeleteProgram(idBindedProgram);
}

/*-----------------------------------------------

Name:	ActivateShader

Params:	none

Result:	runs program with shader efffects

---------------------------------------------*/

void BindShader::ActivateShader()
{
	if (bLinked)
		glUseProgram(idBindedProgram);
}

/*-----------------------------------------------

Name:	GetBindedShaderID

Params:	none

Result:	returns OpenGL generated shader ID

---------------------------------------------*/

UINT BindShader::GetBindedShaderID()
{
	return idBindedProgram;
}