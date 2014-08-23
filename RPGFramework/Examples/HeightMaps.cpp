
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include "GLApp.h"
#include"BindShader.h"

// height map diemnsions 4x4
#define HM_SIZE_X 4 
#define HM_SIZE_Y 4

UINT uVBOHeightMapData; // heightmap data are stored here (vertices)
UINT uVBOIndices; // indices for rendering height map

UINT uVAOHeightmap; // one VAO for heightmap

bool bShowFPS = false;
bool bVSync = true;

Shader vShader, fShader;
BindShader spMain;

void InitScene(LPVOID lpParam)
{
		glClearColor(0.0f, 0.0f, 5.0f, 0.0f);

		// Setup heightmap

		glGenVertexArrays(1, &uVAOHeightmap); // Create one VAO
		glGenBuffers(1, &uVBOHeightMapData); // One VBO for data
		glGenBuffers(1, &uVBOIndices); // And finally one VBO for indices

		glBindVertexArray(uVAOHeightmap);
		glBindBuffer(GL_ARRAY_BUFFER, uVBOHeightMapData);

		glm::vec3 vHeightmapData[HM_SIZE_X*HM_SIZE_Y]; // Here the heightmap vertices will be stored temporarily

		float fHeights[HM_SIZE_X*HM_SIZE_Y] =
		{
			4.0f, 2.0f, 3.0f, 1.0f,
			3.0f, 5.0f, 8.0f, 2.0f,
			7.0f, 10.0f, 12.0f, 6.0f,
			4.0f, 6.0f, 8.0f, 3.0f
		};

		float fSizeX = 40.0f, fSizeZ = 40.0f;

		for (int i = 0; i < HM_SIZE_X*HM_SIZE_Y; i++)
		{
			float column = float(i%HM_SIZE_X), row = float(i / HM_SIZE_X);
			vHeightmapData[i] = glm::vec3(
				-fSizeX / 2 + fSizeX*column / float(HM_SIZE_X - 1), // X Coordinate
				fHeights[i],									// Y Coordinate (it's height)
				-fSizeZ / 2 + fSizeZ*row / float(HM_SIZE_Y - 1)	// Z Coordinate
				);
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*HM_SIZE_X*HM_SIZE_Y, vHeightmapData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &uVBOIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uVBOIndices);
		int iIndices[] =
		{
			0, 4, 1, 5, 2, 6, 3, 7, 16, // First row, then restart
			4, 8, 5, 9, 6, 10, 7, 11, 16, // Second row, then restart
			8, 12, 9, 13, 10, 14, 11, 15, // Third row, no restart
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iIndices), iIndices, GL_STATIC_DRAW);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(HM_SIZE_X*HM_SIZE_Y);

		// Load shaders and create shader program

		vShader.LoadShader("data\\Shaders\\map.vert", GL_VERTEX_SHADER);
		fShader.LoadShader("data\\Shaders\\map.frag", GL_FRAGMENT_SHADER);

		spMain.CreateShader();
		spMain.AddShader(&vShader);
		spMain.AddShader(&fShader);

		spMain.LinkShader();
		spMain.ActivateShader();

		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0);
}

//--------------------------------------------------

float fRotationAngle = 0.0f;
const float PIover180 = 3.1415f / 180.0f;

void RenderScene(LPVOID lpParam)
{
	//typecast lpParam to Render pointer
	Render* glRender = (Render*)lpParam;

	// clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(uVAOHeightmap);

	int iModelViewLoc = glGetUniformLocation(spMain.GetBindedShaderID(), "modelViewMatrix");
	int iProjectionLoc = glGetUniformLocation(spMain.GetBindedShaderID(), "projectionMatrix");
	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*glRender->GetProjectionMatrix()));

	glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 60, 30), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mCurrent = glm::rotate(mModelView, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glBindVertexArray(uVAOHeightmap);
	glDrawElements(GL_TRIANGLE_STRIP, HM_SIZE_X*(HM_SIZE_Y - 1) * 2 + HM_SIZE_Y - 2, GL_UNSIGNED_INT, 0);

	fRotationAngle += appMain.sof(30.0f);

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

	if (Keys::OneKey(VK_ESCAPE))
		PostQuitMessage(0);


	// call SwapBuffers
	glRender->SwapBuffersM();
}

void ReleaseScene(LPVOID lpParam)
{
	spMain.DeleteShader();

	glDeleteBuffers(1, &uVBOHeightMapData);
	glDeleteBuffers(1, &uVBOIndices);
	glDeleteVertexArrays(1, &uVAOHeightmap);

	vShader.DeleteShader();
	fShader.DeleteShader();
}


