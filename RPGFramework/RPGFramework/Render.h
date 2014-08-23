#ifndef RENDER_H
#define RENDER_H

#define GLM_FORCE_RADIANS

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib,"glew32.lib")

#include <glew.h>
#include <wglew.h>
#include <Windows.h>
#include <time.h>
#include <glm.hpp>
#include<gtc/matrix_transform.hpp>

#define OPENGL_WINDOW_NAME "GL_Window"

class Render
{
public:
	bool InitOpenGL(HINSTANCE hInstance, HWND* hWnd, int iMajorVersion, int iMinorVersion, void(*InitScene)(LPVOID), void(*RenderScene)(LPVOID)
		, void(*ReleaseScene)(LPVOID), LPVOID lpParam);

	void ResizeOpenGLViewportFull();
	void SetProjection3D(float fFOV, float fAspectRatio, float fNear, float fFar);
	glm::mat4* GetProjectionMatrix();
	

	void RenderGLWindow(LPVOID lpParam);
	void ReleaseGLWindow(LPVOID lpParam);

	static void RegisterGLWindow(HINSTANCE hInstance);
	static void UnregisterGLWindow(HINSTANCE hInstance);

	void MakeCurrent();
	void SwapBuffersM();

	bool SetVerticalSynchronization(bool bEnabled);

	int GetFPS();

	Render();

private:
	bool InitGLEW(HINSTANCE hInstance);

	HDC		hDC;
	HWND*	hWnd;
	HGLRC	hRC;

	static bool bWindowRegistered;
	static bool bGlewInitialized;
	int iMajorVersion, iMinorVersion;

	// used for fps calculation
	int iFPSCount;
	int iCurrentFPS;
	clock_t tLastSecond;

	//Matrix for perspective projection
	glm::mat4 mProjection;

	void(*InitScene)(LPVOID lpParam), (*RenderScene)(LPVOID lpParam), (*ReleaseScene)(LPVOID lpParam);
};

LRESULT CALLBACK MsgGLWindowHandler(HWND, UINT, WPARAM, LPARAM);

#endif