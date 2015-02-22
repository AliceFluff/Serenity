////////////////////////////////////
////// External Header file's //////


/////////////////////////////////
////// Local Header file's //////
#include <App.hpp>

/////////////////////////////////////
////// External Library file's //////


//////////////////////////////////
////// Local Library file's //////


//////////////////////////////////
////// External Namespace's //////


///////////////////////////////
////// Local Namespace's //////


///////////////////////////
////// Constructor's //////
cApp::cApp(void)
{

}
cApp::cApp(HINSTANCE hInstance, wstring WindowTitle_, UINT ClientWidth_, UINT ClientHeight_, bool ConsoleLogOn_) : cSystem(hInstance, WindowTitle_, ClientWidth_, ClientHeight_, ConsoleLogOn_)
{

}
cApp::~cApp(void)
{

}

//////////////////////
////// Method's //////
// initialise app
bool cApp::InitApp(void)
{
	cSystem::InitApp();
	return true;
}

// resize app
bool cApp::ResizeApp(void)
{
	return true;
}

// update app
bool cApp::UpdateScene(float dt)
{
	static float t_base = 0.0f;
	static int frameCount = 0;
	frameCount++;

	if ((mTime.GetGameTime() - t_base) >= 1.0f)
	{
		float fps = (float)frameCount;
		float mspf = 1000.0f / fps;

		wostringstream output;
		output.precision(6);
		output << WindowTitle.c_str() <<
			L" - FPS: " << fps << " - MSPF: " << mspf;

		SetWindowText(mhWnd, output.str().c_str());
		frameCount = 0;
		t_base += 1.0f;
	}

	if (_UpdateSceneCount <= 0)
	{
		_UpdateSceneCount++;
		SystemLog.ConsoleWriteString(L"cApp::UpdateScene() Success");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	}
	return true;
}

// update app
bool cApp::DrawScene(void)
{
	// clear the back buffer to a deep blue
	mD3D11DeviceContext->ClearRenderTargetView(mD3D11BackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// do 3D rendering on the back buffer here

	// switch the back buffer and the front buffer
	mD3D11SwapChain->Present(0, 0);

	if (_DrawSceneCount <= 0)
	{
		_DrawSceneCount++;
		SystemLog.ConsoleWriteString(L"cApp::UpdateScene() Success");
		SystemLog.OutputLogMessage(SystemLog.ConsoleWrite(SystemLog.ConsoleWriteString(L"")));
	}
	return true;
}