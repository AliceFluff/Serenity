////////////////////////////////////
////// External Header file's //////


/////////////////////////////////
////// Local Header file's //////
#include <Time.hpp>

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
cTime::cTime(void)
{
	__int64 CountsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&CountsPerSec);
	mSecondPerCount = 1.0 / (double)CountsPerSec;
}
cTime::~cTime()
{

}

//////////////////////
////// Method's //////
// return current time
float cTime::GetGameTime()const
{
	if (!mRun)
	{
		return (float)((mStopTime - mBaseTime)*mSecondPerCount);
	}
	else
	{
		return (float)(((mCurrentTime - mPauseTime) - mBaseTime)*mSecondPerCount);
	}
}

// return delta time (time between counts)
float cTime::GetDeltaTime()const
{
	return (float)mDeltaTime;
}

// reset time
void cTime::ResetTime()
{
	__int64 CurrTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);

	mBaseTime = mCurrentTime;
	mPreviousTime = mCurrentTime;
	mPauseTime = 0;
}

// start time
void cTime::StartTime()
{
	__int64 StartTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&StartTime);

	if (!mRun)
	{
		mPauseTime += (StartTime - mStopTime);

		StartTime = StartTime;

		mStopTime = 0;
		mRun = true;
	}
}

// stop time
void cTime::StopTime()
{
	if (!mRun)
	{
		__int64 CurrTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);
		mCurrentTime = CurrTime;

		mStopTime = mCurrentTime;
		mRun = false;
	}
}

// ticks of cpu
void cTime::TickTime()
{
	if (!mRun)
	{
		mDeltaTime = 0.0;
		return;
	}

	__int64 CurrTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);
	mCurrentTime = CurrTime;

	mDeltaTime = (mCurrentTime - mPreviousTime) *mSecondPerCount;

	mPreviousTime = mCurrentTime;

	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}