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
	///<summary>Взима обект на документа</summary>
	CProjectsDocument* GetDocument() const;
	///<summary>Двоен клик за Preview</summary>
	void OnListDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	///<summary>Десен клик за Menu</summary>
	void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	///<summary>Edit опция от меню</summary>
	void OnContextEdit();
	///<summary>Remove опция от меню</summary>
	void OnContextDelete();
	///<summary>Add опция от меню</summary>
	void OnContextInsert();
	///<summary>Refresh опция от меню</summary>
	void OnContextLoad();
	///<summary>Обновява елементите по ListCtrl()</summary>
	void SetItemToListCtrl(int nItem, const PROJECTS& oProject);

	// Overrides
	// ----------------

	///<summary>Функция извикана при инициализация на диалога</summary>
	virtual void OnInitialUpdate();
	///<summary>Функция извикана при промяна по диалога</summary>
	virtual void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* /* pHint */);
};