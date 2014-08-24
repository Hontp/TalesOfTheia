#include"ShaderUtils.h"

ShaderUtil::ShaderUtil()
{
	bLinked = false;
}

/*-----------------------------------------------

Name:	CreateShader

Params:	none

Result:	Creates a new shader

---------------------------------------------*/

void ShaderUtil::CreateShaderProgram()
{
	ProgramID = glCreateProgram();
}

/*-----------------------------------------------

Name:	AddShader

Params:	oShader - shader file

Result:	adds a shader file to program

---------------------------------------------*/

bool ShaderUtil::AddShaderToProgram(Shader* oShader)
{
	if (!oShader->IsLoaded())
		return false;

	glAttachShader(ProgramID, oShader->GetShaderID());

	return true;
}

/*-----------------------------------------------

Name:	LinkShader

Params:	none

Result:	link shader to OpenGL program

---------------------------------------------*/

bool ShaderUtil::LinkShader()
{
	glLinkProgram(ProgramID);
	int iLinkStatus;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &iLinkStatus);
	if (iLinkStatus == GL_TRUE)
		bLinked = true;

	return bLinked;
}

/*-----------------------------------------------

Name:	DeleteShader

Params:	none

Result:	Deletes shader and frees GPU memory

---------------------------------------------*/

void ShaderUtil::DeleteShaderProgram()
{
	if (!bLinked)
		return;

	bLinked = false;
	glDeleteProgram(ProgramID);
}

/*-----------------------------------------------

Name:	ActivateShader

Params:	none

Result:	runs program with shader efffects

---------------------------------------------*/

void ShaderUtil::UseShaderProgram()
{
	if (bLinked)
		glUseProgram(ProgramID);
}

/*-----------------------------------------------

Name:	GetBindedShaderID

Params:	none

Result:	returns OpenGL generated shader ID

---------------------------------------------*/

UINT ShaderUtil::GetShaderProgramID()
{
	return ProgramID;
}