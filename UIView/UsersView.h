#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include <afxcview.h>
#include "UsersDocument.h"
#include "UsersDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersView

class UIViewDLL_EXP CUsersView : public CListView
{
// Macros
// ----------------
	DECLARE_DYNCREATE(CUsersView)
	DECLARE_MESSAGE_MAP()

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
	void OnListDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);

	void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnContextEdit();
	void OnContextDelete();
	void OnContextInsert();
	void OnContextLoad();
// Overrides
// ----------------
	/// <summary>Инициализира данните за CListCtrl()</summary>
	virtual void OnInitialUpdate();

	//TODO: OnUpdate() ...
	virtual void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* /* pHint */);
// Members
// ----------------
};