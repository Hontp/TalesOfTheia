#include "GLApp.h"				// Main Window Class
#include "ShaderUtils.h"		// Shader utility Class
#include "Texture.h"

#include "VBO.h"

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include"geometry.h"

bool bShowFPS = false;
bool bVSync = true;


//create VBO
VertexBufferObject VBOScene;
//create VAO
UINT uVAO;

//create shader binding object
Shader vShader, fShader;
ShaderUtil spShader;

Texture tGold, tSnow;

void InitScene(LPVOID lpParam)
{
	// clear rendering window
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/////Load assets bind shaders here/////

	VBOScene.CreateVBO();
	glGenVertexArrays(1, &uVAO); // Create one VAO
	glBindVertexArray(uVAO);

	VBOScene.BindVBO();

	// Add cube to VBO

	for (int i = 0; i < 36; i++)
	{
		VBOScene.AddData(&vCubeVertices[i], sizeof(glm::vec3));
		VBOScene.AddData(&vCubeTexCoords[i % 6], sizeof(glm::vec2));
	}

	// Add pyramid to VBO

	for (int i = 0; i < 12; i++)
	{
		VBOScene.AddData(&vPyramidVertices[i], sizeof(glm::vec3));
		VBOScene.AddData(&vPyramidTexCoords[i % 3], sizeof(glm::vec2));
	}

	// Add ground to VBO

	for (int i = 0; i < 6; i++)
	{
		VBOScene.AddData(&vGround[i], sizeof(glm::vec3));
		vCubeTexCoords[i] *= 5.0f;
		VBOScene.AddData(&vCubeTexCoords[i % 6], sizeof(glm::vec2));
	}

	VBOScene.LoadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions start on zero index, and distance between two consecutive is sizeof whole
	// vertex data (position and tex. coord)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates start right after positon, thus on (sizeof(glm::vec3)) index,
	// and distance between two consecutive is sizeof whole vertex data (position and tex. coord)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));

	// Load shaders and create shader program

	vShader.LoadShader("data\\Shaders\\scene.vert", GL_VERTEX_SHADER);
	fShader.LoadShader("data\\Shaders\\scene.frag", GL_FRAGMENT_SHADER);

	spShader.CreateShaderProgram();
	spShader.AddShaderToProgram(&vShader);
	spShader.AddShaderToProgram(&fShader);

	spShader.LinkShader();
	spShader.UseShaderProgram();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	// load texture
	tGold.LoadTexture2D("data\\Textures\\gold.jpg", true);
	tGold.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	tSnow.LoadTexture2D("data\\Textures\\snow.jpg", true);
	tSnow.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	glEnable(GL_TEXTURE_2D);
}

float fRotationAngleCube = 0.0f, fRotationAnglePyramid = 0.0f;
float fCubeRotationSpeed = 0.0f, fPyramidRotationSpeed = 0.0f;
const float PIover180 = 3.1415f / 180.0f;

void displayTexFilerInfo()
{
	char buff[255];
	std::string tinfoMinification[] =
	{
		"Nearest",
		"Bilinear"
	};

	std::string tinfoMagnification[] =
	{
		"Nearest",
		"Bilinear",
		"Nearest on closest mipmap",
		"Bilinear on closest mipmap",
		"Trilinear"
	};

	sprintf_s(buff, "Mag Filter: %s Min Filter: %s", tinfoMinification[tSnow.GetMagnificationFilter()].c_str(),
		tinfoMagnification[tSnow.GetMinificationFilter() - 2].c_str());

	SetWindowText(appMain.hWnd, buff);
}

void RenderScene(LPVOID lpParam)
{
	// typecast lpParam to Render pointer
	Render* glRender = (Render*)lpParam;


	////Rendering happens here////

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int iModelViewLoc = glGetUniformLocation(spShader.GetShaderProgramID(), "modelViewMatrix");
	int iProjectionLoc = glGetUniformLocation(spShader.GetShaderProgramID(), "projectionMatrix");
	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*glRender->GetProjectionMatrix()));

	glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 12, 27), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mCurrent;

	glBindVertexArray(uVAO);

	// texture binding set GL_ACTIVE_TEXTURE0, then tellfragment shader
	// that Sampler variable will get data from GL_ACTIVE_TEXTURE0

	int iSimplerLoc = glGetUniformLocation(spShader.GetShaderProgramID(), "gSampler");
	glUniform1i(iSimplerLoc, 0);

	tGold.BindTexture(0);

	// render cube

	mCurrent = glm::translate(mModelView, glm::vec3(-8.0f, 0.0f, 0.0f));
	mCurrent = glm::scale(mCurrent, glm::vec3(10.0f, 10.0f, 10.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngleCube, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Rendering of pyramid

	mCurrent = glm::translate(mModelView, glm::vec3(8.0f, 0.0f, 0.0f));
	mCurrent = glm::scale(mCurrent, glm::vec3(10.0f, 10.0f, 10.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAnglePyramid, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));

	glDrawArrays(GL_TRIANGLES, 36, 12);

	// render ground
	tSnow.BindTexture();

	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mModelView));
	glDrawArrays(GL_TRIANGLES, 48, 6);

	// user control

	if (Keys::Key(VK_UP))fCubeRotationSpeed -= appMain.sof(60.0f);
	if (Keys::Key(VK_DOWN))fCubeRotationSpeed += appMain.sof(60.0f);
	if (Keys::Key(VK_RIGHT))fPyramidRotationSpeed += appMain.sof(60.0f);
	if (Keys::Key(VK_LEFT))fPyramidRotationSpeed -= appMain.sof(60.0f);

	fRotationAngleCube += appMain.sof(fCubeRotationSpeed);
	fRotationAnglePyramid += appMain.sof(fPyramidRotationSpeed);
	
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
	else if (Keys::OneKey(VK_F3))
	{
		tGold.SetFiltering((tGold.GetMagnificationFilter() + 1) % 2, tGold.GetMinificationFilter());
		tSnow.SetFiltering((tSnow.GetMagnificationFilter() + 1) % 2, tSnow.GetMinificationFilter());
		displayTexFilerInfo();
	}
	else if (Keys::OneKey(VK_F4))
	{
		int newMinFilter = tSnow.GetMinificationFilter() == TEXTURE_FILTER_MIN_TRILINEAR ? TEXTURE_FILTER_MIN_NEAREST : tSnow.GetMinificationFilter() + 1;
		tSnow.SetFiltering(tSnow.GetMagnificationFilter(), newMinFilter);
		tGold.SetFiltering(tGold.GetMagnificationFilter(), newMinFilter);
		displayTexFilerInfo();
	}

	if (bShowFPS)
	{
		char buff[55];

		sprintf_s(buff, "FPS: %d, VSync: %s", glRender->GetFPS(), bVSync ? "On" : "Off");

		SetWindowText(appMain.hWnd, buff);
	}

	if (Keys::OneKey(VK_ESCAPE))
		PostQuitMessage(0);


	// call SwapBuffers
	glRender->SwapBuffersM();
}

void ReleaseScene(LPVOID lpParam)
{
	// delete shader program object
	spShader.DeleteShaderProgram();

	// delete VBOs and VAOs here
	VBOScene.ReleaseVBO();
	glDeleteVertexArrays(1, &uVAO);

	//delete shader objects
	vShader.DeleteShader();
	fShader.DeleteShader();
}
