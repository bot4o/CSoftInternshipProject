// Application.h : main header file for the Application DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CApplicationApp
// See Application.cpp for the implementation of this class
//

class CApplicationApp : public CWinApp
{
public:
	CApplicationApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
