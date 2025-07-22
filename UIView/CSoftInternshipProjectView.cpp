#include "pch.h"
#include "framework.h"
#include "CSoftInternshipProjectView.h"

/////////////////////////////////////////////////////////////////////////////
//CCSoftInternshipProjectView

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CCSoftInternshipProjectView, CListView)

BEGIN_MESSAGE_MAP(CCSoftInternshipProjectView, CListView)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
CCSoftInternshipProjectView::CCSoftInternshipProjectView() noexcept
{
}

CCSoftInternshipProjectView::~CCSoftInternshipProjectView()
{
}

// Methods
// ----------------
CCSoftInternshipProjectDocument* CCSoftInternshipProjectView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCSoftInternshipProjectDocument)));
	return (CCSoftInternshipProjectDocument*)m_pDocument;
}


// MFC Message Handlers
// ----------------


//Methods
// ----------------


// Overrides
// ----------------

void CCSoftInternshipProjectView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
}

void CCSoftInternshipProjectView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
}

#ifdef _DEBUG
void CCSoftInternshipProjectView::AssertValid() const
{
	CListView::AssertValid();
}

void CCSoftInternshipProjectView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif 