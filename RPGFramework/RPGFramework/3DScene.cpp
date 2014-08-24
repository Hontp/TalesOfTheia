#include "GLApp.h"				// Main Window Class
#include "ShaderUtils.h"		// Shader utility Class

bool bShowFPS = false;
bool bVSync = true;

// create vertex and fragment shader objects
Shader vShader, fShader;

//create shader binding object
ShaderUtil spShader;

void InitScene(LPVOID lpParam)
{
	// clear rendering window
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/////Load assets bind shaders here/////
}

void RenderScene(LPVOID lpParam)
{
	// typecast lpParam to Render pointer
	Render* glRender = (Render*)lpParam;


	////Rendering happens here////


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
	// delete shader program object
	spShader.DeleteShaderProgram();

	// delete VBOs and VAOs here

	//delete shader objects
	vShader.DeleteShader();
	fShader.DeleteShader();
}
