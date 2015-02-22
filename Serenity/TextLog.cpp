////////////////////////////////////
////// External Header file's //////


/////////////////////////////////
////// Local Header file's //////
#include <TextLog.hpp>

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
cLog::cLog(void)
{
	GetLocalTime(&mSystemTime);

	mVTab = L"\n\n\n\n";
	mHTab = L"\t";
}
cLog::~cLog(void)
{

}

//////////////////////
////// Method's //////
// initalise console (not log)
bool cLog::ConsoleInit(void)
{
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;

	system("Color 1A");
	return true;
}

// set directoy and name for file output
bool cLog::SetDirectryAndName(wstring StrDirectory_, wstring StrFileName_)
{
	mDirectoryName = StrDirectory_;
	mFilename = StrFileName_;

	CreateDirectoryAndLogFile();
	return true;
}

// open the file output target directory and file
bool cLog::OpenDirectory(void)
{
	if (MessageBoxEx(NULL, L"Would you like to see Log Directory?", L"Open Log(s)", MB_ICONQUESTION | MB_YESNO, 0) == IDYES)
	{
		ShellExecute(NULL, L"open", mDirectoryName.c_str(), NULL, NULL, SW_SHOWDEFAULT);
		ShellExecute(NULL, L"open", mFilename.c_str(), NULL, NULL, SW_SHOWDEFAULT);
	}
	return true;
}

// write line to console
wstring cLog::ConsoleWrite(wstring Str)
{
	wstringstream sStr;
	sStr.eof();
	sStr.str(L"");
	sStr << L"\n\n" << DateStamp().c_str() << TimeStamp().c_str() << Str.c_str();
	Str = sStr.str();
	wprintf(Str.c_str());
	return Str.c_str();
}

// write part of string to console
wstring cLog::ConsoleWriteString(wstring Str)
{
	ConsoleWriteString_.eof();
	ConsoleWriteString_ << " " << Str.c_str();
	ConsoleString_ = ConsoleWriteString_.str();
	return ConsoleString_.c_str();
}

// output line to txt file output in target directory
wstring cLog::OutputLogMessage(wstring StrLog)
{
	mOutStream.open(mFilename.c_str());
	if (mOutStream.is_open())
	{
		mOutStream.eof();
		mSStrLogMessage << L"\n\n" << StrLog.c_str();
		StrLog = mSStrLogMessage.str();
		mOutStream << StrLog.c_str();
	}
	else
	{
		ConsoleWrite(L"**** Error OutputLogMessage() Failed ****\n\n");
	}
	mOutStream.close();
	ConsoleWriteString_.str(L"");
	return StrLog.c_str();
}

// date stamp	
wstring cLog::DateStamp(void)
{
	wstringstream mSStrDateStamp;
	mSStrDateStamp.str(L"");
	mSStrDateStamp << setfill(L'0') << setw(2) << mSystemTime.wYear << L"/" << setfill(L'0') << setw(2)
		<< mSystemTime.wMonth << L"/" << setfill(L'0') << setw(2) << mSystemTime.wDay << " ";
	return mSStrDateStamp.str();
}

// time stamp
wstring cLog::TimeStamp(void)
{
	wstringstream mSStrTimeStamp;
	mSStrTimeStamp.str(L"");
	mSStrTimeStamp << setfill(L'0') << setw(2) << mSystemTime.wHour << ":" << setfill(L'0') << setw(2)
		<< mSystemTime.wMinute << ":" << setfill(L'0') << setw(2) << mSystemTime.wSecond << " - ";
	return mSStrTimeStamp.str();
}

// create directory and file
bool cLog::CreateDirectoryAndLogFile(void)
{
	if (!ConsoleInit())
	{
		MessageBox(NULL, L"ConsoleInit() Failed to Initialise!", L"ERROR", MB_ICONERROR);
		return false;
	}

	if (!SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, NULL, Path))
	{
		wstringstream mSStrDirectory;
		mSStrDirectory << Path << mDirectoryName.c_str();
		mDirectoryName = mSStrDirectory.str();
		SHCreateDirectory(NULL, mDirectoryName.c_str());
	}
	else
	{
		ConsoleWriteString(L"**** SHGetFolderPath() Failed to create/find %LOCALAPPDATA% Directory!! ****");
		ConsoleWrite(mDirectoryName.c_str());
		OutputLogMessage(ConsoleWrite(ConsoleWriteString(L"")));
		return false;
	}

	mSStrFilename << mDirectoryName.c_str() << "\\" << mSystemTime.wYear << setfill(L'0') << setw(2)
		<< mSystemTime.wMonth << setfill(L'0') << setw(2) << mSystemTime.wDay << "_" << setfill(L'0') << setw(2)
		<< mSystemTime.wHour << setfill(L'0') << setw(2) << mSystemTime.wMinute << setfill(L'0') << setw(2)
		<< mSystemTime.wSecond << "_" << mFilename.c_str();
	mFilename = mSStrFilename.str();

	ConsoleWriteString(L"Initialising Console and Text Log.\n\n");
	ConsoleWriteString(L"SetDirectryAndName() Success: \n");
	ConsoleWriteString(mFilename.c_str());
	ConsoleWriteString(mVTab);
	OutputLogMessage(ConsoleWrite(ConsoleWriteString(L"")));
	return true;
}