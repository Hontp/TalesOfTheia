#include "Render.h"


bool Render::bWindowRegistered = false;
bool Render::bGlewInitialized = false;

Render::Render()
{
	iFPSCount = 0;
	iCurrentFPS = 0;
}

/*---------------------------------------------
Name: InitGLEW

Params: none

Result: Creates window and OpenGL context and 
initialized GLEW
----------------------------------------------*/

bool Render::InitGLEW(HINSTANCE hInstance)
{
	if (bGlewInitialized)return true;

	RegisterGLWindow(hInstance);

	HWND hWndFake = CreateWindow(OPENGL_WINDOW_NAME, "DUMMY", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, hInstance, NULL);

	hDC = GetDC(hWndFake);

	// First, choose false pixel format

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (iPixelFormat == 0)return false;

	if (!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

	// Create the false, old style context (OpenGL 2.1 and before)

	HGLRC hRCFake = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRCFake);

	bool bResult = true;

	if (!bGlewInitialized)
	{
		if (glewInit() != GLEW_OK)
		{
			MessageBox(*hWnd, "Couldn't initialize GLEW!", "Fatal Error", MB_ICONERROR);
			bResult = false;
		}
		bGlewInitialized = true;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCFake);
	DestroyWindow(hWndFake);

	return bResult;
}

/*---------------------------------------------
Name: InitOpenGL

Params: hInstance - application instance
		dhWnd	- window to init OpenGL to
		iMajorVersion - major verion of OpenGL
		iMinorVersion - minor version of OpenGL
		InitScene - pointer to init function
		RenderScene - pointer to render function
		ReleaseScene - optional parameter of 
					   release function



Result: Initializes OpenGL rendering context
of specified version, if sceeds return true
----------------------------------------------*/


bool Render::InitOpenGL(HINSTANCE hInstance, HWND* dhWnd, int iMajorVersion, int iMinorVersion,
	void(*g_InitScene)(LPVOID), void(*g_RenderScene)(LPVOID), void(*g_ReleaseScene)(LPVOID), LPVOID lpParam)
{
	if (!InitGLEW(hInstance))return false;

	hWnd = dhWnd;
	hDC = GetDC(*hWnd);

	bool bError = false;
	PIXELFORMATDESCRIPTOR pfd;

	if (iMajorVersion <= 2)
	{
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
		if (iPixelFormat == 0)return false;

		if (!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

		// Create the old style context (OpenGL 2.1 and before)
		hRC = wglCreateContext(hDC);
		if (hRC)wglMakeCurrent(hDC, hRC);
		else bError = true;
	}
	else if (WGLEW_ARB_create_context && WGLEW_ARB_pixel_format)
	{
		const int iPixelFormatAttribList[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0 // End of attributes list
		};
		int iContextAttribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, iMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, iMinorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0 // End of attributes list
		};

		int iPixelFormat, iNumFormats;
		wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

		// PFD seems to be only redundant parameter now
		if (!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

		hRC = wglCreateContextAttribsARB(hDC, 0, iContextAttribs);
		// If everything went OK
		if (hRC) wglMakeCurrent(hDC, hRC);
		else bError = true;
	}

	if (bError)
	{
		MessageBox(*hWnd, "Current version of Open GL not supported", "Fatal Error", MB_ICONERROR);
		return false;
	}

	RenderScene		= g_RenderScene;
	InitScene		= g_InitScene;
	ReleaseScene	= g_ReleaseScene;

	if (InitScene != NULL)InitScene(lpParam);
	
	return true;
}


/*---------------------------------------------
Name: ResizeOpenGLViewPortFull

Params: none

Result: Resizes viewport to  full window
perspective projection
----------------------------------------------*/

void Render::ResizeOpenGLViewportFull()
{
	if (hWnd == NULL) return;
	RECT rRect;
	GetClientRect(*hWnd, &rRect);
	glViewport(0, 0, rRect.right, rRect.bottom);
}

/*----------------------------------------------
Name: SetProjection3D

Params : fFOV - field of view
		fAspsectRatio = aspect ratio of width/ height
		fNear, - distance of near and far clipping plane


Result : Calculates projection matrix
----------------------------------------------*/
void Render::SetProjection3D(float fFOV, float fAspectRatio, float fNear, float fFar)
{
	mProjection = glm::perspective(fFOV, fAspectRatio, fNear, fFar);
}

glm::mat4* Render::GetProjectionMatrix()
{
	return &mProjection;
}


/*---------------------------------------------
Name: RegisterGLWindow

Params: hInstance - application instance

Result: Register OpenGL Window
----------------------------------------------*/

void Render::RegisterGLWindow(HINSTANCE hInstance)
{
	if (bWindowRegistered)return;
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)MsgGLWindowHandler;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = OPENGL_WINDOW_NAME;

	RegisterClassEx(&wc);

	bWindowRegistered = true;
}


/*---------------------------------------------
Name: UnregisterGLWindow

Params: hInstance - application instance

Result: unregister opengl window
----------------------------------------------*/

void Render::UnregisterGLWindow(HINSTANCE hInstance)
{
	if (bWindowRegistered)
	{
		UnregisterClass(OPENGL_WINDOW_NAME, hInstance);
		bWindowRegistered = false;
	}
}


/*---------------------------------------------
Name: MsgGLWindowHandler

Params: window messages

Result: handles messages from windows that use
opengl window class
----------------------------------------------*/
LRESULT CALLBACK MsgGLWindowHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (uMsg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}


/*---------------------------------------------
Name: SwapBufferM

Params: none

Result: swap back and front buffer
----------------------------------------------*/
void Render::SwapBuffersM()
{
	SwapBuffers(hDC);
}


/*---------------------------------------------
Name: MakeCurrent

Params: none

Result: Makes current device and opengl rendering
context to those associated with OpenGL
control
----------------------------------------------*/

void Render::MakeCurrent()
{
	wglMakeCurrent(hDC, hRC);
}


/*---------------------------------------------
Name: RenderGLWindow

Params: lpparam - pointer to window object

Result: Calls previously set render function
----------------------------------------------*/

void Render::RenderGLWindow(LPVOID lpParam)
{
	clock_t tCurrent = clock();
	if ((tCurrent - tLastSecond) >= CLOCKS_PER_SEC)
	{
		tLastSecond += CLOCKS_PER_SEC;
		iFPSCount = iCurrentFPS;
		iCurrentFPS = 0;
	}

	if (RenderScene)RenderScene(lpParam);
	iCurrentFPS++;
}

/*-------------------------------------------- -
Name: GetFPS

Params : none
Result : return FPS count
----------------------------------------------*/
int Render::GetFPS()
{
	return iFPSCount;
}


/*---------------------------------------------
Name: GetFPS

Params: lpParam - pointer to window Object

Result: Calls preivously set release function
and deletes rendering context
----------------------------------------------*/

void Render::ReleaseGLWindow(LPVOID lpParam)
{
	if (ReleaseScene)ReleaseScene(lpParam);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(*hWnd, hDC);

	hWnd = NULL;
}

/*---------------------------------------------
Name: SetVerticalSynronization

Params : bEabled - V-Sync on or off

Result : toggle V-Sync on/off
----------------------------------------------*/

bool Render::SetVerticalSynchronization(bool bEnabled)
{
	if (!wglSwapIntervalEXT)
		return false;

	if (bEnabled)
		wglSwapIntervalEXT(1);
	else
		wglSwapIntervalEXT(0);

	return true;
}