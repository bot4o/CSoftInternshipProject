#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"

class ClientApp : public CWinApp
{
public:
	ClientApp() noexcept;

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern ClientApp theApp;