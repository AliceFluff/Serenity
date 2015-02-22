////////////////////////////////////
////// External Header file's //////


/////////////////////////////////
////// Local Header file's //////
#include <System.hpp>

/////////////////////////////////////
////// External Library file's //////


//////////////////////////////////
////// Local Library file's //////


//////////////////////////////////
////// External Namespace's //////


///////////////////////////////
////// Local Namespace's //////

/////////////////////////////
////// Global Method's //////
// window procedure
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message)
	{
		return App->MsgProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

///////////////////////////
////// Constructor's //////
cSystem::cSystem(void)
{

}
cSystem::cSystem(HINSTANCE hInstance, wstring WindowTitle_, UINT ClientWidth_, UINT ClientHeight_, bool ConsoleLogOn_)
{
	// Window's Members	
	mhWnd = NULL;
	mhInstance = hInstance;
	mClientWidth = ClientWidth_;
	mClientHeight = ClientHeight_;
	WindowTitle = WindowTitle_.c_str();
	wr = { 0, 0, mClientWidth, mClientHeight };

	// false true
	ConsoleLogOnOff(ConsoleLogOn_);
}
cSystem::~cSystem(void)
{

}

//////////////////////
////// Method's //////
// Decide whether console/log is on/off
bool cSystem::ConsoleLogOnOff(bool ConsoleLogOn_)
{
	if (ConsoleLogOn_ == true)
	{
		if (MessageBoxEx(NULL, L"Would you like Console Log on?", L"Console Log", MB_ICONQUESTION | MB_YESNO, 0) == IDYES)
		{
			// Open a text log
			ConsoleLogOn = true;
			InitConsoleLog(L"\\Serenity Log(s)", L"Serenity System Log.txt");
		}
		else
		{
			ConsoleLogOn = false;
		}

	}
	return true;
}

// cSystem local message procedure / main loop
int cSystem::Run(void)
{
	MSG msg = { 0 };
	mTime.ResetTime();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mTime.TickTime();
			UpdateApp(mTime.GetDeltaTime());
		}
	}
	ShutdownWindow();
	return static_cast<int>(msg.wParam);
}

// initialise the app
bool cSystem::InitApp(void)
{
	if (!InitWindow(wr.right - wr.left, wr.bottom - wr.top, 0, 0, L"Engine_01", WS_OVERLAPPEDWINDOW))
	{
		SystemLog.ConsoleWriteString(L"cSystem::InitApp() Failed");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
		ERROR(L"cSystem::InitApp() Failed");
		return false;
	}

	SystemLog.ConsoleWriteString(L"cSystem::InitApp() Success");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

// resize app
bool cSystem::ResizeApp(void)
{
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

// upade app
bool cSystem::UpdateApp(float dt)
{
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

// cSystem local message procedure
LRESULT cSystem::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		return 0x0;
	} break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0x0;
	} break;

	case WM_ENTERSIZEMOVE:
	{
		mResizing = true;
		mTime.StopTime();
		return 0x0;
	} break;

	case WM_EXITSIZEMOVE:
	{
		ResizeApp();
		mResizing = false;
		mTime.StartTime();
		return 0x0;
	} break;

	case WM_GETMINMAXINFO:
	{
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 800;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 600;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = 1440;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = 900;
		return 0x0;
	} break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// initialise log(s)
bool cSystem::InitConsoleLog(wstring StrDirectory_, wstring StrFileName_)
{
	if (!SystemLog.SetDirectryAndName(StrDirectory_.c_str(), StrFileName_.c_str()))
	{
		ERROR(L"cSystem::InitConsoleLog() Failed");
		return false;
	}
	SystemLog.ConsoleWriteString(L"cSystem::InitConsoleLog() Success");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

// initalise main window
bool cSystem::InitWindow(UINT width, UINT height, UINT x, UINT y, wstring WindowClassName, DWORD mWindowStyle)
{
	// Window Class Discription
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = mhInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(mhInstance, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(mhInstance, IDI_APPLICATION);
	wcex.hbrBackground = (HBRUSH)GetStockBrush(BLACK_BRUSH);
	wcex.lpszClassName = WindowClassName.c_str();

	if (!RegisterClassEx(&wcex))
	{
		SystemLog.ConsoleWriteString(L"RegisterClassEx(&wcex) Failed");
		ERROR(L"RegisterClassEx() Failed \n");
		return false;
	}

	AdjustWindowRect(&wr, mWindowStyle, FALSE);
	x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	y = GetSystemMetrics(SM_CYSCREEN) / 2 - width / 2;

	mhWnd = CreateWindow(WindowClassName.c_str(), WindowTitle.c_str(), mWindowStyle,
		x, y, width, height, NULL, NULL, mhInstance, NULL);
	if (!mhWnd)
	{
		SystemLog.ConsoleWriteString(L"CreateWindow() Failed - mhWnd = NULL");
		ERROR(L"CreateWindow() Failed \n");
		return false;
	}

	if (ShowWindow(mhWnd, SW_SHOW))
	{
		SystemLog.ConsoleWriteString(L"ShowWindow() Failed");
		ERROR(L"ShowWindow() Failed \n");
		return false;
	}
	if (!UpdateWindow(mhWnd))
	{
		SystemLog.ConsoleWriteString(L"UpdateWindow() Failed");
		ERROR(L"UpdateWindow() Failed \n");
		return false;
	}

	SystemLog.ConsoleWriteString(L"cSystem::InitWindow() Success");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

// Shut down the main window
bool cSystem::ShutdownWindow(void)
{
	if (ConsoleLogOn)
	{
		if (!SystemLog.OpenDirectory())
		{
			SystemLog.ConsoleWriteString(L"SystemLog.OpenDirectory() Failed - Application Shutting Down!");
			SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
			ERROR(L"SystemLog.OpenDirectory() Failed \n");
			return false;
		}
	}
	SystemLog.ConsoleWriteString(L"SystemLog.OpenDirectory() Success\n\n**** Application Shut Down Successfully ****");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}