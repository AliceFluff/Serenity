//////////////////////////////////////
////// Preprocessor directive's //////
#ifndef _TIME_HPP_
#define _TIME_HPP_

////////////////////////////////////
////// External Header file's //////
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <sstream>
#include <string> 

/////////////////////////////////
////// Local Header file's //////


/////////////////////////////////////
////// External Library file's //////


//////////////////////////////////
////// Local Library file's //////


//////////////////////////////////
////// External Namespace's //////


///////////////////////////////
////// Local Namespace's //////
using namespace std;

////////////////////////
////// Time Class //////
class cTime
{
public:
	////// Constructor's //////
	cTime(void);
	~cTime(void);

	////// Time Methods's //////
	float GetGameTime(void)const;				// return current time
	float GetDeltaTime(void)const;				// return delta time (time between counts)
	float m_Fps = 0.0f;							// FPS storage

	void ResetTime(void);						// reset time
	void StartTime(void);						// start time
	void StopTime(void);						// stop time
	void TickTime(void);						// ticks of cpu

protected:
	////// Time Member's //////
	bool mRun = true;							// are we running...?

	__int64 mBaseTime = 0;						// base time
	__int64 mCurrentTime = 0;					// current time
	__int64 mPreviousTime = 0;					// previous time
	__int64 mPauseTime = 0;						// paused time
	__int64 mStopTime = 0;						// stop time

	double mDeltaTime = 0.0;					// store time delta
	double mSecondPerCount = 0.00;				// how many seconds in a count/tick
};

#endif // !_TIME_HPP_