#pragma once
#include <afxcview.h>
#include "DLLExport.h"
#include "UsersDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersView

class UIViewDLL_EXP CUsersView : public CListView
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CUsersView)
	

	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CUsersView();
	virtual ~CUsersView();


	// Methods
	// ----------------
	CUsersDocument* GetDocument() const;


	// Overrides
	// ----------------
	/// <summary>Инициализира данните за CListCtrl()</summary>
	virtual void OnInitialUpdate();

	//TODO: OnUpdate() ...
	virtual void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* /* pHint */);
	// Members
	// ----------------

};