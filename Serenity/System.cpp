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
	////// Class Member's //////
	result = NULL;
	UpdateSceneCount = 0;
	DrawSceneCount = 0;	

	////// Subsystem's //////
	ConsoleLogOn = false;	

	////// Win 32 Member's //////
	mhWnd = NULL;
	mhInstance = hInstance;	
	WindowTitle = WindowTitle_.c_str();
	wr = { 0, 0, ClientWidth_, ClientHeight_ };
	mClientWidth = wr.right - wr.left;
	mClientHeight = wr.bottom - wr.top;

	ConsoleLogOnOff(true, true);
}
cSystem::~cSystem(void)
{

}

//////////////////////
////// Method's //////
// Decide whether console/log is on/off
bool cSystem::ConsoleLogOnOff(bool ConsoleLogOn_, bool TextLogOn_)
{
	ConsoleLogOn = true;

	if (ConsoleLogOn_)
		SystemLog.OpenConsole();

	if (TextLogOn_)
		SystemLog.OpenTextLog(WindowTitle.c_str(), L"Serenity_Log");

	return true;
}

// cSystem local message procedure / main loop
int cSystem::Run(void)
{
	SystemLog.WriteLine(L" -- cSystem::Run() Begin -- ", 1, 1, 0); 

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
					SystemLog.WriteLine(L"cSystem::Run() - UpdateScene() Success", 1, 1, 0);
				}
			}

			if (DrawScene())
			{
				if (DrawSceneCount <= 0)
				{
					DrawSceneCount++;
					SystemLog.WriteLine(L"cSystem::Run() - DrawScene() Success", 1, 1, 0);
				}
			}
		}
	}

	if (!ShutdownDirect3D11())
		SystemLog.WriteLine(L"cSystem::Run() - ShutdownDirect3D11() Failed", 1, 1, 1);
	if(!ShutdownWindow())
		SystemLog.WriteLine(L"cSystem::Run() - ShutdownWindow() Failed", 1, 1, 1);

	SystemLog.WriteLine(L" -- cSystem::Run() Success -- ", 1, 1, 0);
	return static_cast<int>(msg.wParam);
}

// initialise the app
bool cSystem::InitApp(void)
{
	SystemLog.WriteLine(L" -- cSystem::InitApp() Begin -- ", 1, 1, 0);

	if (!InitWindow(mClientWidth, mClientHeight, 0, 0, L"Engine_01", WS_OVERLAPPEDWINDOW))
	{
		SystemLog.WriteLine(L"cSystem::InitApp() - InitWindow() Failed", 1, 1, 1);
		return false;
	}

	if (!InitDirect3D11(mClientWidth, mClientHeight, 0, 0))
	{
		SystemLog.WriteLine(L"cSystem::InitApp() - InitDirect3D11() Failed", 1, 1, 1);
		return false;
	}

	SystemLog.WriteLine(L" -- cSystem::InitApp() - InitDirect3D11() Success -- ", 1, 1, 0);
	return true;
}

// resize app
bool cSystem::ResizeApp(void)
{
	SystemLog.WriteLine(L" -- cSystem::ResizeApp() Begin -- ", 1, 1, 0);



	SystemLog.WriteLine(L" -- cSystem::ResizeApp() Success -- ", 1, 1, 0);
	return true;
}

// upade app
bool cSystem::UpdateScene(float dt)
{	
	SystemLog.WriteLine(L" -- cSystem::UpdateScene() Begin -- ", 1, 1, 0);



	SystemLog.WriteLine(L" -- cSystem::UpdateScene() Success --", 1, 1, 0);
	return true;
}

bool cSystem::DrawScene(void)
{
	SystemLog.WriteLine(L" -- cSystem::DrawScene() Begin -- ", 1, 1, 0);



	SystemLog.WriteLine(L" -- cSystem::DrawScene() Success -- ", 1, 1, 0);
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

// initalise main window
bool cSystem::InitWindow(UINT width, UINT height, UINT x, UINT y, wstring WindowClassName, DWORD mWindowStyle)
{
	SystemLog.WriteLine(L" -- cSystem::InitWindow() Begin -- ", 1, 1, 0);

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
		SystemLog.WriteLine(L"cSystem::InitWindow() - RegisterClassEx() Failed", 1, 1, 1);
		return false;
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitWindow() - RegisterClassEx() Success", 1, 1, 0);
	}

	if(!AdjustWindowRect(&wr, mWindowStyle, FALSE))
	{
		SystemLog.WriteLine(L"cSystem::InitWindow() - AdjustWindowRect() Failed", 1, 1, 1);
		return false;
	}
	else
	{
		x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		y = GetSystemMetrics(SM_CYSCREEN) / 2 - width / 2;
		SystemLog.WriteLine(L"cSystem::InitWindow() - AdjustWindowRect() Success", 1, 1, 0);
	}
	
	if((mhWnd = CreateWindow(WindowClassName.c_str(), WindowTitle.c_str(), mWindowStyle,
		x, y, width, height, NULL, NULL, mhInstance, NULL)) == NULL)
	{
		SystemLog.WriteLine(L"cSystem::InitWindow() - CreateWindow() Failed", 1, 1, 1);
		return false;
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitWindow() - CreateWindow() Success", 1, 1, 0);
	}

	if (ShowWindow(mhWnd, SW_SHOW))
	{
		SystemLog.WriteLine(L"cSystem::InitWindow() - ShowWindow() Failed", 1, 1, 1);
		return false;
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitWindow() - ShowWindow() Success", 1, 1, 0);
	}
	if (!UpdateWindow(mhWnd))
	{
		SystemLog.WriteLine(L"cSystem::InitWindow() - UpdateWindow() Failed", 1, 1, 1);
		return false;
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitWindow() - UpdateWindow() Success", 1, 1, 0);
	}

	SystemLog.WriteLine(L" -- cSystem::InitWindow() Success -- ", 1, 1, 0);
	return true;
}

// Shut down the main window
bool cSystem::ShutdownWindow(void)
{
	SystemLog.WriteLine(L" -- cSystem::ShutdownWindow() Begin -- ", 1, 1, 0);

	if (ConsoleLogOn)
	{
		if (!SystemLog.OpenDirectory())
		{
			SystemLog.WriteLine(L"cSystem::ShutdownWindow() - SystemLog.OpenDirectory() Failed", 1, 1, 1);
			return false;
		}
	}

	SystemLog.WriteLine(L" -- cSystem::ShutdownWindow() Success -- ", 1, 1, 0);
	return true;
}

bool cSystem::InitDirect3D11(UINT width, UINT height, UINT x, UINT y)
{	
	SystemLog.WriteLine(L" -- cSystem::InitDirect3D11() Begin -- ", 1, 1, 0);

	// Create swapchain information
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;										// one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// how swap chain is to be used
	scd.OutputWindow = mhWnd;									// the window to be used
	scd.SampleDesc.Count = 4;									// how many multisamples
	scd.Windowed = TRUE;										// windowed/full-screen mode
	
	if (FAILED(result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, NULL,
		NULL, NULL,
		D3D11_SDK_VERSION,
		&scd, &mD3D11SwapChain, &mD3D11Device,
		NULL, &mD3D11DeviceContext)))
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - D3D11CreateDeviceAndSwapChain() Failed", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - D3D11CreateDeviceAndSwapChain() Success", 1, 1, 0);
	}

	// Create Swap Chain	

	// Create the Device Context

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	if (FAILED(result = mD3D11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - mD3D11SwapChain->GetBuffer() Failed", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - mD3D11SwapChain->GetBuffer() Success", 1, 1, 0);
	}

	// use the back buffer address to create the render target
	if (FAILED(result = mD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &mD3D11BackBuffer)))
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - mD3D11Device->CreateRenderTargetView() Failed", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - mD3D11Device->CreateRenderTargetView() Success", 1, 1, 0);
	}
	if (pBackBuffer)
	{
		pBackBuffer->Release();
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - pBackBuffer->Release() Success", 1, 1, 0);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - pBackBuffer->Release() Failed", 1, 1, 1);
	}
	

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

	SystemLog.WriteLine(L" -- cSystem::InitDirect3D11() Success -- ", 1, 1, 0);
	return true;
}

bool cSystem::ShutdownDirect3D11(void)
{
	SystemLog.WriteLine(L" -- cSystem::ShutdownDirect3D11() Begin -- ", 1, 1, 0);

	// close and release all existing COM objects
	if (mD3D11SwapChain)
	{
		mD3D11SwapChain->Release();
		SystemLog.WriteLine(L"cSystem::ShutdownDirect3D11() - mD3D11SwapChain->Release() Success", 1, 1, 0);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::ShutdownDirect3D11() - mD3D11SwapChain->Release() Failed", 1, 1, 1);
	}

	if (mD3D11BackBuffer)
	{
		mD3D11BackBuffer->Release();
		SystemLog.WriteLine(L"cSystem::ShutdownDirect3D11() - mD3D11BackBuffer->Release() Success", 1, 1, 0);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::ShutdownDirect3D11() - mD3D11BackBuffer->Release() Failed", 1, 1, 1);
	}		

	if (mD3D11SwapChain)
	{
		mD3D11Device->Release();
		SystemLog.WriteLine(L"cSystem::ShutdownDirect3D11() - mD3D11Device->Release() Success", 1, 1, 0);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::ShutdownDirect3D11() - mD3D11Device->Release() Failed", 1, 1, 1);
	}
		
	if (mD3D11SwapChain)
	{
		mD3D11DeviceContext->Release();
		SystemLog.WriteLine(L"cSystem::ShutdownDirect3D11() - mD3D11DeviceContext->Release() Success", 1, 1, 0);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::ShutdownDirect3D11() - mD3D11DeviceContext->Release() Failed", 1, 1, 1);
	}
	SystemLog.WriteLine(L" -- cSystem::ShutdownDirect3D11() Success -- ", 1, 1, 0);
	return true;
}