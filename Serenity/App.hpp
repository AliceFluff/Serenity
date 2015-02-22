//////////////////////////////////////
////// Preprocessor directive's //////
#ifndef _APP_HPP_
#define _APP_HPP_

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
using namespace std;

///////////////////////////////
////// Local Namespace's //////


///////////////////////////////
////// Application Class //////
class cApp : public cSystem
{
	//////////////////////////////////////////////
	////// Public Methods - Class interface //////
public:
	////// Constructor's //////
	cApp::cApp(void);
	cApp(HINSTANCE hInstance, wstring WindowTitle_, UINT ClientWidth_, UINT ClientHeight_, bool ConsoleLogOn_);
	~cApp(void);

	////// Public Method's //////
	bool InitApp(void) override;				// initialise app
	bool ResizeApp(void) override;				// resize app
	bool UpdateApp(float dt) override;			// update app
};

#endif	// _APP_HPP_