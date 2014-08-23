#include "GLApp.h"

GLAPP	appMain;

char Keys::kp[256] = { 0 };

/*--------------------------------------
Name : Key

Params: iKey = virtual key code

Result: Return true of key is pressed

----------------------------------------*/

int Keys::Key(int iKey)
{
	return (GetAsyncKeyState(iKey) >> 15) & 1;
}

/*----------------------------------------
Name: Onekey

Params: iKeys virutal key code

Results: Return if was pressed, but only
once ( key is released )

------------------------------------------*/

int Keys::OneKey(int iKey)
{
	if (Key(iKey) && !kp[iKey])
	{
		kp[iKey] = 1;
		return 1;
	}

	if (!Key(iKey)) kp[iKey] = 0;
	return 0;
}

/*------------------------------------------
Name: ResetTimer

Params: none

Result: resets application time

--------------------------------------------*/

void GLAPP::ResetTimer()
{
	tLastFrame = clock();
	frameInterval = 0.0f;
}

/*------------------------------------------
Name: UpdateTimer

Params: none

Result: Update application time

--------------------------------------------*/
void GLAPP::UpdateTimer()
{
	clock_t tCur = clock();
	frameInterval = float(tCur - tLastFrame)/float(CLOCKS_PER_SEC);
	tLastFrame = tCur;
}


/*--------------------------------------------

Name: sof

Params: fVal

Result: speed optimized float

----------------------------------------------*/

float GLAPP::sof(float fVal)
{
	return fVal*frameInterval;
}

/*---------------------------------------------
Name: InitializeApp

Params: appName

Result: Initilize app with specfied Name
----------------------------------------------*/

bool GLAPP::InitializeApp(std::string appName)
{
	sAppName = appName;
	hMutex = CreateMutex(NULL, 1, sAppName.c_str());
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "This Application is already running", "Multiple Instances Found", MB_ICONINFORMATION | MB_OK);
		return 0;
	}

	return 1;
}

/*---------------------------------------------
Name: RegisterAppWindow

Params: hInstance - application instance handle

Result: Registers application window

----------------------------------------------*/

LRESULT CALLBACK GlobalMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return appMain.MsgHandlerMain(hWnd, uMsg, wParam, lParam);
}

void GLAPP::RegisterAppWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hInstance = hInstance;

	wcex.lpfnWndProc = GlobalMessageHandler;
	wcex.lpszClassName = sAppName.c_str();

	wcex.lpszMenuName = NULL;

	RegisterClassEx(&wcex);
}

/*-----------------------------------------------
Name: CreateAppWindow

Params: sTitle = title of window

Result: create nmain application window

-------------------------------------------------*/

bool GLAPP::CreateAppWindow(std::string sTitle)
{
	if (MessageBox(NULL, "Run in fullscreen ?", "Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		DEVMODE dmSettings = { 0 };
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSettings); // get current display settings

		hWnd = CreateWindowEx(0, sAppName.c_str(), sTitle.c_str(), WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			0, 0, dmSettings.dmPelsWidth, dmSettings.dmPelsHeight, NULL, NULL, hInstance, NULL);
	}
	else
	{
		hWnd = CreateWindowEx(0, sAppName.c_str(), sTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
			0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	}

		if (!glRender.InitOpenGL(hInstance, &hWnd, 3, 3, InitScene, RenderScene, ReleaseScene, &glRender)) 
			return false;

		ShowWindow(hWnd, SW_SHOW);

		ShowWindow(hWnd, SW_SHOWMAXIMIZED);
		UpdateWindow(hWnd);

	return true;
}

/*--------------------------------------------------
Name: AppBody

Params: None

Result: Main application body infinite Loop

----------------------------------------------------*/

void GLAPP::AppBody()
{
	MSG msg;

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)break; // if the message was WM_QUIT exit application
			else
			{
				TranslateMessage(&msg); // otherwise send message to appropriate window
				DispatchMessage(&msg);
			}
		}
		else if (bAppActive)
		{
			UpdateTimer();
			glRender.RenderGLWindow(&glRender);
		}
		else Sleep(200); // do not consume processor power of application isn't active
	}
}

/*-------------------------------------------------
Name: Shutdown

Params: none

Result: Shuts down application and releases used
memory

---------------------------------------------------*/

void GLAPP::ShutDown()
{
	glRender.ReleaseGLWindow(&glRender);

	DestroyWindow(hWnd);
	UnregisterClass(sAppName.c_str(), hInstance);
	Render::UnregisterGLWindow(hInstance);
	ReleaseMutex(hMutex);
}

/*----------------------------------------------------
Name: MsgHandlerMain

Params: window message

Result: application message handler
------------------------------------------------------*/

LRESULT CALLBACK GLAPP::MsgHandlerMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_ACTIVATE:
	{
		switch (LOWORD(wParam))
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			bAppActive = true;
			break;
		case WA_INACTIVE:
			bAppActive = false;
			break;
		}
		break;
	}
	case WM_SIZE:
		glRender.ResizeOpenGLViewportFull();
		glRender.SetProjection3D(45.0f, float(LOWORD(lParam)) / float(HIWORD(lParam)), 0.001f, 1000.0f);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

/*------------------------------------------------------------
Name: getInstance

Params: none

Result: Returns application instance

--------------------------------------------------------------*/

HINSTANCE GLAPP::GetInstance()
{
	return hInstance;
}

/*------------------------------------------------------------
Name: WinMain

Params: none

Result: main application entry point

--------------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
	if (!appMain.InitializeApp("GL_Window"))
		return 0;
	appMain.RegisterAppWindow(hInstance);

	if (!appMain.CreateAppWindow("Test Open GL Window"))
		return 0;

	appMain.ResetTimer();

	appMain.AppBody();
	appMain.ShutDown();

	return 0;
}