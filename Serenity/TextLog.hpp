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

///////////////////////
////// Log Class //////
class cLog
{
	///////////////////////////////////////////
	////// Log Methods - Class interface //////
public:
	////// Constructor's //////
	cLog(void);
	~cLog(void);

	////// Log Methods's //////
	bool ConsoleInit(void);						// initalise console (not log)
	bool SetDirectryAndName(wstring StrDirectory_, wstring StrFileName_);	// set directoy and name for file output
	bool OpenDirectory(void);					// open the file output target directory and file

	wstring ConsoleWrite(wstring Str);			// write line to console
	wstring ConsoleWriteString(wstring Str);	// write part of string to console
	wstring OutputLogMessage(wstring StrLog);	// output line to txt file output in target directory

private:
	////// Member's //////
	SYSTEMTIME mSystemTime;						// system time represention
	wofstream mOutStream;						// stream out text to file

	TCHAR Path[512];							// directory path
	wstring mDirectoryName;						// complete directory path

	wstringstream mSStrFilename;				// file name
	wstring mFilename;							// complete file name

	wstringstream mSStrLogMessage;				// string stream for log message
	wstringstream ConsoleWriteString_;			// string stream for console message
	wstring ConsoleString_;						// string for console message

	wstring mVTab;								// virticle tab
	wstring mHTab;								// horizontal tab

	////// Method's //////
	wstring DateStamp(void);					// time stamp
	wstring TimeStamp(void);					// date stamp	

	bool CreateDirectoryAndLogFile(void);
};

#endif // !_LOG_HPP_