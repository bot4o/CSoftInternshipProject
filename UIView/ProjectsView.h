#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include <afxcview.h> 
#include "../Document/ProjectsAndTasksDocument.h"

//#include "ProjectsDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CProjectsView

class UIViewDLL_EXP CProjectsView : public CListView
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CProjectsView)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CProjectsView();
	virtual ~CProjectsView();

	// Methods
	// ----------------
	CProjectsAndTasksDocument* GetDocument() const;
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
	virtual void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* /* pHint */);

	// Members
	// ----------------
};