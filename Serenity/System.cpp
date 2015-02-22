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
	hr = NULL;
	UpdateSceneCount = 0;
	DrawSceneCount = 0;
	// Window's Members	
	mhWnd = NULL;
	mhInstance = hInstance;	
	WindowTitle = WindowTitle_.c_str();
	wr = { 0, 0, ClientWidth_, ClientHeight_ };
	mClientWidth = wr.right - wr.left;
	mClientHeight = wr.bottom - wr.top;

	// false true
	if (!ConsoleLogOnOff(ConsoleLogOn_))
		ERROR(L"cSystem::ConsoleLogOnOff() Failed");
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
			if (!InitConsoleLog(L"\\Serenity Log(s)", L"Serenity System Log.txt"))
				ERROR(L"cSystem::ConsoleLogOnOff() Failed to Initialise cSystem::InitConsoleLog()");
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

			if (UpdateScene(mTime.GetDeltaTime()))
			{
				if (UpdateSceneCount <= 0)
				{
					UpdateSceneCount++;
					SystemLog.ConsoleWriteString(L"cSystem::UpdateScene() Success");
					SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));					
				}
			}

			if (DrawScene())
			{
				if (DrawSceneCount <= 0)
				{
					DrawSceneCount++;
					SystemLog.ConsoleWriteString(L"cSystem::DrawScene() Success");
					SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
				}
			}
		}
	}
	if (!ShutdownDirect3D11())
		ERROR(L"cSystem::ShutdownDirect3D11() Failed");
	if(!ShutdownWindow())
		ERROR(L"cSystem::ShutdownWindow() Failed");	
	return static_cast<int>(msg.wParam);
}

// initialise the app
bool cSystem::InitApp(void)
{
	if (!InitWindow(mClientWidth, mClientHeight, 0, 0, L"Engine_01", WS_OVERLAPPEDWINDOW))
	{
		SystemLog.ConsoleWriteString(L"cSystem::InitWindow() Failed");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
		ERROR(L"cSystem::InitWindow() Failed");
		return false;
	}

	if (!InitDirect3D11(mClientWidth, mClientHeight, 0, 0))
	{
		SystemLog.ConsoleWriteString(L"cSystem::InitDirect3D11() Failed");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
		ERROR(L"cSystem::InitDirect3D11() Failed");
		return false;
	}

	SystemLog.ConsoleWriteString(L"cSystem::InitApp() Success");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

// resize app
bool cSystem::ResizeApp(void)
{
	SystemLog.ConsoleWriteString(L"cSystem::ResizeApp() Success");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

// upade app
bool cSystem::UpdateScene(float dt)
{	

	return true;
}

bool cSystem::DrawScene(void)
{
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

	case WM_MENUCHAR:
	{
		return MAKELRESULT(0, MNC_CLOSE);
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
		SystemLog.ConsoleWriteString(L"RegisterClassEx() Failed");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
		ERROR(L"RegisterClassEx() Failed \n");
		return false;
	}

	if(!AdjustWindowRect(&wr, mWindowStyle, FALSE))
	{
		SystemLog.ConsoleWriteString(L"AdjustWindowRect() Failed");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
		ERROR(L"AdjustWindowRect() Failed \n");
		return false;
	}
	else
	{
		x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		y = GetSystemMetrics(SM_CYSCREEN) / 2 - width / 2;
	}
	
	if((mhWnd = CreateWindow(WindowClassName.c_str(), WindowTitle.c_str(), mWindowStyle,
		x, y, width, height, NULL, NULL, mhInstance, NULL)) == NULL)
	{
		SystemLog.ConsoleWriteString(L"CreateWindow() Failed - mhWnd = NULL");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
		ERROR(L"CreateWindow() Failed \n");
		return false;
	}

	if (ShowWindow(mhWnd, SW_SHOW))
	{
		SystemLog.ConsoleWriteString(L"ShowWindow() Failed");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
		ERROR(L"ShowWindow() Failed \n");
		return false;
	}
	if (!UpdateWindow(mhWnd))
	{
		SystemLog.ConsoleWriteString(L"UpdateWindow() Failed");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
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
			SystemLog.ConsoleWriteString(L"cSystem::ShutdownWindow() Failed - Application Shutting Down!");
			SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
			ERROR(L"cSystem::ShutdownWindow() Failed \n");
			return false;
		}
	}
	SystemLog.ConsoleWriteString(L"cSystem::ShutdownWindow() Success\n\n**** Application Shut Down Successfully ****");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

bool cSystem::InitDirect3D11(UINT width, UINT height, UINT x, UINT y)
{	
	// Create swapchain information
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;										// one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// how swap chain is to be used
	scd.OutputWindow = mhWnd;									// the window to be used
	scd.SampleDesc.Count = 4;									// how many multisamples
	scd.Windowed = TRUE;										// windowed/full-screen mode
	
	if (FAILED( hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, NULL,
		NULL, NULL,
		D3D11_SDK_VERSION,
		&scd, &mD3D11SwapChain, &mD3D11Device,
		NULL, &mD3D11DeviceContext)))
	{
		SystemLog.ConsoleWriteString(L"cSystem::InitDirect3D11() - D3D11CreateDeviceAndSwapChain() Call Failed");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	}

	// Create Swap Chain	

	// Create the Device Context

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	mD3D11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	mD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &mD3D11BackBuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	mD3D11DeviceContext->OMSetRenderTargets(1, &mD3D11BackBuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));

	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = mClientWidth;
	ViewPort.Height = mClientHeight;

	mD3D11DeviceContext->RSSetViewports(1, &ViewPort);

	SystemLog.ConsoleWriteString(L"cSystem::InitDirect3D11() Success");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}

bool cSystem::ShutdownDirect3D11(void)
{
	// close and release all existing COM objects
	if (mD3D11SwapChain)
		mD3D11SwapChain->Release();

	if (mD3D11BackBuffer)
		mD3D11BackBuffer->Release();

	if (mD3D11SwapChain)
		mD3D11Device->Release();

	if (mD3D11SwapChain)
		mD3D11DeviceContext->Release();

	SystemLog.ConsoleWriteString(L"cSystem::ShutdownDirect3D11() Success");
	SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	return true;
}