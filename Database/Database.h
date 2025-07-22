// Database.h : main header file for the Database DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDatabaseApp
// See Database.cpp for the implementation of this class
//

class CDatabaseApp : public CWinApp
{
public:
	CDatabaseApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
