#pragma once
#include <afxcview.h>
#include "DLLExport.h"
#include <afx.h>
#include "CSoftInternshipProjectDoc.h"

/////////////////////////////////////////////////////////////////////////////
//CCSoftInternshipProjectView

class UIViewDLL_EXP CCSoftInternshipProjectView : public CListView
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CCSoftInternshipProjectView)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CCSoftInternshipProjectView() noexcept;
	virtual ~CCSoftInternshipProjectView();

	// Methods
	// ----------------
public:
	CCSoftInternshipProjectDocument* GetDocument() const;

	// Overrides
	// ----------------
protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* /* pHint */);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Members
	// ----------------
};