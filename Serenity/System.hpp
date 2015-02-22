// Preprocessor Directives
#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#define WIN32_LEAN_AND_MEAN	// set windows to bear minimum

/////////////////////////////////////
////// Error Reporting Macro's //////
#define ERROR(m_Msg)(MessageBox(NULL, m_Msg, L"ERROR!", MB_ICONERROR));
#define SUCCESS(m_Msg)(MessageBox(NULL, m_Msg, L"SUCCESS!", MB_ICONWARNING));
#define INFO(m_Msg)(MessageBox(NULL, m_Msg, L"INFO!", MB_ICONINFORMATION));

////////////////////////////////////
////// External Header file's //////
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <io.h>
#include <iostream>
#include <sstream>
#include <string> 
#include <fcntl.h>
#include <wchar.h>

/////////////////////////////////
////// Local Header file's //////
#include <Time.hpp>
#include <TextLog.hpp>

/////////////////////////////////////
////// External Library file's //////


//////////////////////////////////
////// Local Library file's //////


//////////////////////////////////
////// External Namespace's //////
using namespace std;

///////////////////////////////
////// Local Namespace's //////


/////////////////////////////////////////////////////////////////////////////////////////////
////// Main System Class - Holds all the information for Window's API and DirectX API. //////
class cSystem
{
	///////////////////////////////////////////////
	////// Public Method's - Class interface //////
public:
	////// Constructor's //////
	cSystem(void);
	cSystem(HINSTANCE hInstance, wstring WindowTitle_, UINT ClientWidth_, UINT ClientHeight_, bool ConsoleLogOn_);
	virtual ~cSystem(void);

	////// Main loop //////
	bool ConsoleLogOnOff(bool ConsoleLogOn_);
	int Run(void);								// cSystem local message procedure / main loop

	////// Get Spcific Value(s) //////
	HINSTANCE GetApphInstance(void);			// get hInstance
	HWND GetApphWnd(void);						// get hWnd

	////// Public Method's //////
	virtual bool InitApp(void);					// initialise the app
	virtual bool ResizeApp(void);				// resize app
	virtual bool UpdateApp(float dt);			// upade app
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	// cSystem local message procedure

protected:
	/////////////////////////
	////// Subsystem's //////

	////// Log's //////
	cLog SystemLog;								// log object
	bool ConsoleLogOn;							// log on/off switch
	bool InitConsoleLog(wstring StrDirectory_, wstring StrFileName_);	// initialise log(s)

	////// Time Representation //////
	cTime mTime;	// high precision time object

	////////////////////
	////// Win 32 //////
protected:
	////// Win 32 Member's //////
	HINSTANCE mhInstance;						// main window instance
	HWND mhWnd;									// main window handle	
	UINT mClientWidth;							// window client width
	UINT mClientHeight;							// window client height
	wstring WindowTitle;						// window title
	RECT wr;									// RECT structure for client area

	bool mAppPaused;							// app is paused
	bool mMinimized;							// app is minimised
	bool mMaximized;							// app is maximised
	bool mResizing;								// app is currently resizing

	////// Win 32 Method's //////
	bool InitWindow(UINT width, UINT height, UINT x, UINT y, wstring WindowClassName, DWORD mWindowStyle);	// initalise main window
	bool ShutdownWindow(void);					// shut down the main window

	/////////////////////////
	////// Direct3D 11 //////
protected:
};

//////////////////////////////
////// Static Gloabal's //////
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);	// window procedure
static cSystem* App = 0;						// system object

#endif // !_SYSTEM_HPP_