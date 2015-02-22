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
bool cApp::UpdateApp(float dt)
{
	return true;
}