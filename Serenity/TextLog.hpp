// Preprocessor Directives
#ifndef _LOG_HPP_
#define _LOG_HPP_

////////////////////////////////////
////// External Header file's //////
#include <stdio.h>
#include <iostream>
#include <string> 
#include <fstream>
#include <windows.h>
#include <wchar.h>
#include <iomanip>
#include <sstream>
#include <shlobj.h>
#include <fcntl.h>
#include <io.h>
#include <shellapi.h>

/////////////////////////////////
////// Local Header file's //////


/////////////////////////////////////
////// External Library file's //////
#pragma comment(lib, "shell32")

//////////////////////////////////
////// Local Library file's //////


//////////////////////////////////
////// External Namespace's //////


///////////////////////////////
////// Local Namespace's //////
using namespace std;

/////////////////////////////////////////////
////// Error Reporting Message Macro's //////
#define ERRORMSG(m_Msg)(MessageBox(NULL, m_Msg, L"INFO!", MB_ICONERROR));
#define SUCCESSMSG(m_Msg)(MessageBox(NULL, m_Msg, L"SUCCESS!", MB_ICONINFORMATION));
#define INFOMSG(m_Msg)(MessageBox(NULL, m_Msg, L"INFO!", MB_ICONWARNING));

///////////////////////
////// Log Class //////
class cLog
{
	///////////////////////////////////////////
	////// Log Methods - Class interface //////
public:
	////// Constructor's //////
	cLog(void);														// Constructor
	~cLog(void);													// Deconstructor

	////// Log Methods's //////
	bool ConsoleInit(void);											// initalise console KEEPING
	bool TextLogInit(wstring szDirectory_, wstring szFileName_);	// initalise texst log KEEPING

	bool OpenConsole(void);											// open console window up during run time
	bool OpenTextLog(wstring szDirectory, wstring szFilename);		// open text file for recording output to file
	bool OpenDirectory(void);										// open the file output target directory and file

	wstring WriteLine(wstring msg, bool console, bool textlog, UINT msgbox);		// get message, write 1 line and print


private:
	////// Member's //////
	UINT mCon = 0;
	UINT mTxt = 0;

	SYSTEMTIME mSystemTime;											// system time represention
	wofstream mOutStream;											// stream out text to file

	TCHAR Path[1024];												// directory path buffer
	wstring mszLogDirectoryName;									// complete directory path
	wstring mszsFileName;											// Text File Neame
	wstringstream mszsTextOut;										// Text output for text file log
	wstring mVTab;													// virticle tab
	wstring mHTab;													// horizontal tab

	////// Method's //////
	wstring DateStamp(void);										// time stamp
	wstring TimeStamp(void);										// date stamp	

	bool PrintMsgToConsole(wstring msg);							// print line to console
	bool PrintMsgTotTextLog(wstring StrLog);						// print line to text file
};

#endif // !_LOG_HPP_