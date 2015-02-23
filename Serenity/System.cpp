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
	ConsoleLogOn = true;

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

///////////////////////////////////////////////
////// Public Method's - Class interface //////
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

	GetClientRect(mhWnd, &wr);
	mClientWidth = wr.right - wr.left;
	mClientHeight = wr.bottom - wr.top;

	if (mD3D11SwapChain != NULL)
	{
		assert(mD3D11DeviceContext);
		assert(mD3D11Device);
		assert(mD3D11SwapChain);		

		// Release the old views, as they hold references to the buffers we
		// will be destroying.  Also release the old depth/stencil buffer.

		if (mD3D11RenderTargetView)
		{
			mD3D11RenderTargetView->Release();
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11RenderTargetView->Release() Success", 1, 1, 0);
		}
		else
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11RenderTargetView->Release() Failed", 1, 1, 1);
		}
			
		if (mD3DDepthStencilView)
		{
			mD3DDepthStencilView->Release();
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3DDepthStencilView->Release() Success", 1, 1, 0);
		}
		else
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3DDepthStencilView->Release() Failed", 1, 1, 1);
		}
			
		if (mD3DDepthStencilBuffer)
		{
			mD3DDepthStencilBuffer->Release();
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3DDepthStencilBuffer->Release() Success", 1, 1, 0);
		}
		else
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3DDepthStencilBuffer->Release() Failed", 1, 1, 1);
		}

		// Resize the swap chain and recreate the render target view.
		if (FAILED(result = mD3D11SwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11SwapChain->ResizeBuffers() Failed", 1, 1, 1);
		}
		else
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11SwapChain->ResizeBuffers() Success", 1, 1, 0);
		}
		
		// get the address of the back buffer
		if (FAILED(result = mD3D11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mD3D11RenderTargetView)))
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11SwapChain->GetBuffer() Failed", 1, 1, 1);
		}
		else
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11SwapChain->GetBuffer() Success", 1, 1, 0);
		}
		// use the back buffer address to create the render target
		if (FAILED(result = mD3D11Device->CreateRenderTargetView(mD3D11BackBuffer, NULL, &mD3D11RenderTargetView)))
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11SwapChain->CreateRenderTargetView() Failed", 1, 1, 1);
		}
		else
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11SwapChain->CreateRenderTargetView() Success", 1, 1, 0);
		}
		if (FAILED(result = mD3D11BackBuffer->Release()))
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11BackBuffer->Release() Failed", 1, 1, 1);
		}
		else
		{
			SystemLog.WriteLine(L"cSystem::ResizeApp() - mD3D11BackBuffer->Release() Success", 1, 1, 0);
		}

		ZeroMemory(&dsv, sizeof(D3D11_TEXTURE2D_DESC));
		dsv.Width = mClientWidth;
		dsv.Height = mClientHeight;
		dsv.MipLevels = 1;
		dsv.ArraySize = 1;
		dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsv.SampleDesc.Count = 1;
		dsv.SampleDesc.Quality = 0;
		dsv.Usage = D3D11_USAGE_DEFAULT;
		dsv.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsv.CPUAccessFlags = 0;
		dsv.MiscFlags = 0;

		/*// depth stencil view description
		dsv.Width = (mClientWidth);	// size of the depth buffer width
		dsv.Height = (mClientHeight);	// size of the depth buffer height
		dsv.MipLevels = 1;	// mip levels
		dsv.ArraySize = 1;	// array size
		dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// format

		// Use 4X MSAA? --must match swap chain MSAA values.
		if (m_4xMsaaQuality)
		{
			dsv.SampleDesc.Count = 4;
			dsv.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		// No MSAA
		else
		{
			dsv.SampleDesc.Count = SampleDescCount;
			dsv.SampleDesc.Quality = SampleDescQuality;
		}

		dsv.Usage = D3D11_USAGE_DEFAULT;	// how the buffer will be used
		dsv.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// its  a depth stencil buffer
		dsv.CPUAccessFlags = 0;	// cpu access
		dsv.MiscFlags = 0;	// other*/

		result = mD3D11Device->CreateTexture2D(&dsv, NULL, &mD3D11BackBuffer);
		result = mD3D11Device->CreateDepthStencilView(mD3DDepthStencilBuffer, NULL, &mD3DDepthStencilView);

		// set the render target as the back buffer
		mD3D11DeviceContext->OMSetRenderTargets(1, &mD3D11RenderTargetView, mD3DDepthStencilView);

		// Set the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (mClientWidth);
		viewport.Height = (mClientHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		mD3D11DeviceContext->RSSetViewports(1, &viewport);
	}

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

/////////////////////////
////// Subsystem's //////
// Decide whether console/log is on/off
bool cSystem::ConsoleLogOnOff(bool ConsoleLogOn_, bool TextLogOn_)
{
	if (ConsoleLogOn_)
		SystemLog.OpenConsole();

	if (TextLogOn_)
	{
		SystemLog.OpenTextLog(WindowTitle.c_str(), L"Serenity_Log");
	}
	return true;
}

////////////////////
////// Win 32 //////
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
	Sleep(50);
	SystemLog.WriteLine(L" -- cSystem::ShutdownWindow() Success -- ", 1, 1, 0);
	return true;
}

//////////////////
////// DXGI //////
// initialise the DXGI 
bool cSystem::DXGIInit(void)
{
	SystemLog.WriteLine(L" -- cSystem::DXGIInit() Begin -- ", 1, 1, 0);

	// GET HOLD THE INTERFACES
	mD3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&mDXGIDevice);
	if (mDXGIDevice == NULL)
	{
		SystemLog.WriteLine(L"cSystem::DXGIInit() - mDXGIDevice NULL", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::DXGIInit() - mDXGIDevice Acquired Successfully", 1, 1, 0);
	}

	mDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&mDXGIAdapter);
	if (mDXGIAdapter == NULL)
	{
		SystemLog.WriteLine(L"cSystem::DXGIInit() - mDXGIAdapter NULL", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::DXGIInit() - mDXGIAdapter Acquired Successfully", 1, 1, 0);
	}

	mDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&mDXGIFactory);
	if (mDXGIFactory == NULL)
	{
		SystemLog.WriteLine(L"cSystem::DXGIInit() - mDXGIFactory NULL", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::DXGIInit() - mDXGIFactory Acquired Successfully", 1, 1, 0);
	}

	SystemLog.WriteLine(L" -- cSystem::DXGIInit() Success -- ", 1, 1, 0);
	return true;
}

/////////////////////////
////// Direct3D 11 //////
bool cSystem::InitDirect3D11(UINT width, UINT height, UINT x, UINT y)
{	
	SystemLog.WriteLine(L" -- cSystem::InitDirect3D11() Begin -- ", 1, 1, 0);

	UINT createDeviceFlags = 0;
#if defined (DEBUG) || (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if (FAILED(result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&mD3D11Device, NULL, &mD3D11DeviceContext)))
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - D3D11CreateDevice() Failed", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - D3D11CreateDevice() Success", 1, 1, 0);
	}

	if (!DXGIInit())
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - DXGIInit() Failed", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - DXGIInit() Success", 1, 1, 0);
	}

	// Create swapchain information
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;										// one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// how swap chain is to be used
	scd.OutputWindow = mhWnd;									// the window to be used
	scd.SampleDesc.Count = 4;									// how many multisamples
	scd.Windowed = TRUE;										// windowed/full-screen mode	

	// Create Swap Chain	
	if (FAILED(result = mDXGIFactory->CreateSwapChain(mD3D11Device, &scd, &mD3D11SwapChain)))
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - CreateSwapChain() Failed", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - CreateSwapChain() Success", 1, 1, 0);
	}


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
	if (FAILED(result = mD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &mD3D11RenderTargetView)))
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

	if (!ResizeApp())
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - ResizeApp() Failed", 1, 1, 1);
	}
	else
	{
		SystemLog.WriteLine(L"cSystem::InitDirect3D11() - ResizeApp() Success", 1, 1, 0);
	}

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

	if (mD3D11RenderTargetView)
	{
		mD3D11RenderTargetView->Release();
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