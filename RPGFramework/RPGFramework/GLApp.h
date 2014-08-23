#ifndef GL_APP
#define GL_APP

#include <string>
#include "Render.h"

class GLAPP
{
public:
	HWND hWnd; // handle to application  window
	Render glRender; // opengl rendering object

	void ResetTimer();
	void UpdateTimer();
	float sof(float fVal);

	bool InitializeApp(std::string appName);
	void RegisterAppWindow(HINSTANCE hAppInstance);
	bool CreateAppWindow(std::string sTitle);

	void AppBody();
	void ShutDown();

	HINSTANCE GetInstance();

	LRESULT	CALLBACK MsgHandlerMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE		hInstance; // application instance
	std::string		sAppName;
	HANDLE			hMutex;

	bool	bAppActive; // check if application is active
	clock_t	tLastFrame;
	float	frameInterval;
};

namespace Keys
{
	int Key(int Key);
	int OneKey(int iKey);
	extern char kp[256];
}

extern GLAPP appMain;

void InitScene(LPVOID);
void RenderScene(LPVOID);
void ReleaseScene(LPVOID);
#endif