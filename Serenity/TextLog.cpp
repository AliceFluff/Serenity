/////////////////////////////////
////// Local Header file's //////
#include <TextLog.hpp>

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

// initalise console KEEPING
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

	WriteLine(L"cLog Console on.", 1, 0, 0);

	return true;
}

// initalise texst log KEEPING
bool cLog::TextLogInit(wstring szDirectory_, wstring szFileName_)
{	
	if (!SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, NULL, Path))
	{
		wstringstream buffer;
		buffer << Path << "\\" << szDirectory_.c_str();
		szDirectory_ = buffer.str();
		SHCreateDirectory(NULL, szDirectory_.c_str());
		mszLogDirectoryName = szDirectory_.c_str();
	}
	else
	{
		ERRORMSG(L"**** SHGetFolderPath() Failed to create/find %LOCALAPPDATA% Directory!! ****");
		return false;
	}

	wstringstream szsFilenameBuffer;
	szsFilenameBuffer << szDirectory_.c_str() << "\\" << mSystemTime.wYear << setfill(L'0') << setw(2)
		<< mSystemTime.wMonth << setfill(L'0') << setw(2) << mSystemTime.wDay << "_" << setfill(L'0') << setw(2)
		<< mSystemTime.wHour << setfill(L'0') << setw(2) << mSystemTime.wMinute << setfill(L'0') << setw(2)
		<< mSystemTime.wSecond << "_" << szFileName_.c_str() << ".txt";
	mszsFileName = szsFilenameBuffer.str();

	if (mTxt <= 0)
		mTxt = 1;

	WriteLine(L"cLog Text File on.", 1, 1, 0);

	return true;
}

// get message, write 1 line and print
wstring cLog::WriteLine(wstring msg, bool console, bool textlog, UINT msgbox)
{
	wstringstream szsbuffer;
	szsbuffer << DateStamp().c_str() << TimeStamp().c_str() << msg.c_str();
	msg = szsbuffer.str();

	switch (msgbox)
	{
	case 1:
		if (console)
			PrintMsgToConsole(msg.c_str());

		if (textlog)
			PrintMsgTotTextLog(msg.c_str());

		ERRORMSG(msg.c_str());
		break;

	case 2:
		if (console)
			PrintMsgToConsole(msg.c_str());

		if (textlog)
			PrintMsgTotTextLog(msg.c_str());

		SUCCESSMSG(msg.c_str());
		break;

	case 3:
		if (console)
			PrintMsgToConsole(msg.c_str());

		if (textlog)
			PrintMsgTotTextLog(msg.c_str());

		INFOMSG(msg.c_str());
		break;

	default:
		if (console)
			PrintMsgToConsole(msg.c_str());

		if (textlog)
			PrintMsgTotTextLog(msg.c_str());
		break;
	}
	

	return msg.c_str();
}

// print line to console
bool cLog::PrintMsgToConsole(wstring msg)
{
	wstringstream szsbuffer;
	szsbuffer.eof();
	szsbuffer.str(L"");
	szsbuffer << L"\n\n" << msg.c_str();
	msg = szsbuffer.str();
	wprintf(msg.c_str());
	return true;
}

// print line to text file
bool cLog::PrintMsgTotTextLog(wstring StrLog)
{
	mOutStream.open(mszsFileName.c_str());
	if (mOutStream.is_open())
	{
		mOutStream.eof();
		mszsTextOut << L"\n\n" << StrLog.c_str();
		StrLog = mszsTextOut.str();
		mOutStream << StrLog.c_str();
	}
	mOutStream.close();
	return true;
}

// open console window up during run time
bool cLog::OpenConsole(void)
{
	if (MessageBox(NULL, L"Do you want to see the Console?", L"Question", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		ConsoleInit();
		WriteLine(L"Console Active", 1, 0, 0);
	}
	return true;
}

// open text file for recording output to file
bool cLog::OpenTextLog(wstring szDirectory, wstring szFilename)
{
	if (MessageBox(NULL, L"Do you want to generate a text log?", L"Question", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		TextLogInit(szDirectory.c_str(), szFilename.c_str());
		WriteLine(L"Text Log Active", 1, 1, 0);
	}
	return true;
}

// open the file output target directory and file
bool cLog::OpenDirectory(void)
{
	if (MessageBoxEx(NULL, L"Would you like to see Log Directory?", L"Open Log(s)", MB_ICONQUESTION | MB_YESNO, 0) == IDYES)
	{
		WriteLine(L"Showing Directory and Text Log File.", 1, 1, 0);

		ShellExecute(NULL, L"open", mszLogDirectoryName.c_str(), NULL, NULL, SW_SHOWDEFAULT);
		ShellExecute(NULL, L"open", mszsFileName.c_str(), NULL, NULL, SW_SHOWDEFAULT);
	}
	else
	{
		WriteLine(L"Not Showing Directory and Text Log File.", 1, 1, 0);
	}	
	return true;
}

// date stamp	
wstring cLog::DateStamp(void)
{
	wstringstream szsBuffer;
	szsBuffer.str(L"");
	szsBuffer << setfill(L'0') << setw(2) << mSystemTime.wYear << L"/" << setfill(L'0') << setw(2)
		<< mSystemTime.wMonth << L"/" << setfill(L'0') << setw(2) << mSystemTime.wDay << " ";
	return szsBuffer.str();
}

// time stamp
wstring cLog::TimeStamp(void)
{
	wstringstream szsBuffer;
	szsBuffer.str(L"");
	szsBuffer << setfill(L'0') << setw(2) << mSystemTime.wHour << ":" << setfill(L'0') << setw(2)
		<< mSystemTime.wMinute << ":" << setfill(L'0') << setw(2) << mSystemTime.wSecond << " - ";
	return szsBuffer.str();
}

