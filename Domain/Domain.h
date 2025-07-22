// Domain.h : main header file for the Domain DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDomainApp
// See Domain.cpp for the implementation of this class
//

class CDomainApp : public CWinApp
{
public:
	CDomainApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
