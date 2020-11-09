
// PlanetryMotion.h : main header file for the PlanetryMotion application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CPlanetryMotionApp:
// See PlanetryMotion.cpp for the implementation of this class
//

class CPlanetryMotionApp : public CWinApp
{
public:
	CPlanetryMotionApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPlanetryMotionApp theApp;
