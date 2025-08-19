#pragma once
#include "DLLExport.h"
#include "framework.h"
#include "pch.h"
#include "afxcview.h"
#include "../Document/ProjectsDocument.h"

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
	CProjectsDocument* GetDocument() const;
	void OnListDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnContextEdit();
	void OnContextDelete();
	void OnContextInsert();
	void OnContextLoad();
	void SetItemToListCtrl(int nItem, const PROJECTS& oProject);
	// Overrides
	// ----------------
		/// <summary>Инициализира данните за CListCtrl()</summary>
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* /* pHint */);

	// Members
	// ----------------
private:
};