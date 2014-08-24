#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<cmath>
#include"GLApp.h"
#include"ShaderUtils.h"

/*------------------------------------------
Name: InitScene

Params: lpParam Pointer to OpenGL Control

Result: Initilizes OpenGL features that
will be used

--------------------------------------------*/

/*sample polygon*/
float fPyramid[36]; // data to render 4 triangles of 3 vertices of 3 floats
float fPyramidColor[36]; // same for color


UINT uVBO[2]; // one vertex buffer object for vertices positions, one for color
UINT uVAO[1]; // one vertex array object for pyramid

bool bShowFPS = false;
bool bVSync = true;

Shader VertexShader, FragmentShader;
ShaderUtil spShader;

void InitScene(LPVOID lpParam)
{
	// for now set backcolor to blue
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	/*draw object here*/
	
	//setup pyramid object

	// Front face
	fPyramid[0] = 0.0f; fPyramid[1] = 5.0f; fPyramid[2] = 0.0f;
	fPyramid[3] = -3.0f; fPyramid[4] = 0.0f; fPyramid[5] = 3.0f;
	fPyramid[6] = 3.0f; fPyramid[7] = 0.0f; fPyramid[8] = 3.0f;

	// Back face
	fPyramid[9] = 0.0f; fPyramid[10] = 5.0f; fPyramid[11] = 0.0f;
	fPyramid[12] = 3.0f; fPyramid[13] = 0.0f; fPyramid[14] = -3.0f;
	fPyramid[15] = -3.0f; fPyramid[16] = 0.0f; fPyramid[17] = -3.0f;

	// Left face
	fPyramid[18] = 0.0f; fPyramid[19] = 5.0f; fPyramid[20] = 0.0f;
	fPyramid[21] = -3.0f; fPyramid[22] = 0.0f; fPyramid[23] = -3.0f;
	fPyramid[24] = -3.0f; fPyramid[25] = 0.0f; fPyramid[26] = 3.0f;

	// Right face
	fPyramid[27] = 0.0f; fPyramid[28] = 5.0f; fPyramid[29] = 0.0f;
	fPyramid[30] = 3.0f; fPyramid[31] = 0.0f; fPyramid[32] = 3.0f;
	fPyramid[33] = 3.0f; fPyramid[34] = 0.0f; fPyramid[35] = -3.0f;


	// setup pyramid color

	for (int i = 0; i < 4; i++)
	{
		fPyramidColor[i * 9] = 1.0f; fPyramidColor[i * 9 + 1] = 0.0f; fPyramidColor[i * 9 + 2] = 0.0f;
		if (i < 2)
		{
			fPyramidColor[i * 9 + 1] = 0.0f; fPyramidColor[i * 9 + 4] = 1.0f; fPyramidColor[i * 9 + 5] = 0.0f;
			fPyramidColor[i * 9 + 2] = 0.0f; fPyramidColor[i * 9 + 7] = 0.0f; fPyramidColor[i * 9 + 8] = 1.0f;
		}
		else
		{
			fPyramidColor[i * 9 + 2] = 0.0f; fPyramidColor[i * 9 + 7] = 1.0f; fPyramidColor[i * 9 + 8] = 0.0f;
			fPyramidColor[i * 9 + 1] = 0.0f; fPyramidColor[i * 9 + 4] = 0.0f; fPyramidColor[i * 9 + 5] = 1.0f;
		}

	}

	glGenVertexArrays(1, uVAO);
	glGenBuffers(2, uVBO);

	// Setup whole pyramid
	glBindVertexArray(uVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), fPyramid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), fPyramidColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// load and apply shaders to scene
	VertexShader.LoadShader("data\\Shaders\\shader.vert", GL_VERTEX_SHADER);
	FragmentShader.LoadShader("data\\Shaders\\shader.frag", GL_FRAGMENT_SHADER);

	spShader.CreateShaderProgram();
	spShader.AddShaderToProgram(&VertexShader);
	spShader.AddShaderToProgram(&FragmentShader);

	spShader.LinkShader();
	spShader.UseShaderProgram();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

}

/*------------------------------------------
Name: RenderScene

Params: lpParam - Pointer to OpenGL control

Result: Renders scene

--------------------------------------------*/

float fRotationAngle = 0.0f;
const float PIover180 = 3.1415f / 180.0f;

void RenderScene(LPVOID lpParam)
{
	//typecast lpParam to Render pointer
	Render* glRender = (Render*)lpParam;

	// clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(uVAO[0]);

	int iModelViewLoc = glGetUniformLocation(spShader.GetShaderProgramID(), "modelViewMatrix");
	int iProjectionLoc = glGetUniformLocation(spShader.GetShaderProgramID(), "projectionMatrix");
	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*glRender->GetProjectionMatrix()));

	glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 15, 40), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//render rotating pramid in the middle

	glm::mat4 mCurrent = glm::rotate(mModelView, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// render translation praminds

	// left pyramind
	mCurrent = glm::translate(mModelView, glm::vec3(-20.0f, 10.0f*float(sin(fRotationAngle*PIover180)), 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// right pyramid
	mCurrent = glm::translate(mModelView, glm::vec3(20.0f, -10.0f*float(sin(fRotationAngle*PIover180)), 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// translating and rotating top pyramid
	mCurrent = glm::translate(mModelView, glm::vec3(20.0f*float(sin(fRotationAngle*PIover180)), 10.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// final pyramid scaling
	float fScaleValue = 1.5f + float(sin(fRotationAngle*PIover180))*0.5f;
	mCurrent = glm::translate(mModelView, glm::vec3(0.0f, -10.0f, 0.0f));
	mCurrent = glm::scale(mCurrent, glm::vec3(fScaleValue, fScaleValue, fScaleValue));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	fRotationAngle += appMain.sof(120.0f);

	// toggle FPS and V-Sync

	if (Keys::OneKey(VK_F1))
	{
		bShowFPS = !bShowFPS;
		if (!bShowFPS)
		{
			SetWindowText(appMain.hWnd, "Test Open GL Window");
		}
	}

	if (Keys::OneKey(VK_F2))
	{
		bVSync = !bVSync;
		glRender->SetVerticalSynchronization(bVSync);
	}

	if (bShowFPS)
	{
		char buff[55];

		sprintf_s(buff, "FPS: %d, VSync: %s", glRender->GetFPS(), bVSync ? "On" : "Off");

		SetWindowText(appMain.hWnd, buff);
	}

	
	// call SwapBuffers
	glRender->SwapBuffersM();
}

/*------------------------------------------
Name: ReleaseScene

Params: lpParam - Pointer to OpenGL control

Result: Release scene

--------------------------------------------*/

void ReleaseScene(LPVOID lpParam)
{
	spShader.DeleteShaderProgram();

	glDeleteBuffers(2, uVBO);
	glDeleteVertexArrays(1, uVAO);

	VertexShader.DeleteShader();
	FragmentShader.DeleteShader();
}