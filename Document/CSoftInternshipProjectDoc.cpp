#include "pch.h"
#include "CSoftInternshipProjectDoc.h"

/////////////////////////////////////////////////////////////////////////////
//CCSoftInternshipProjectDocument

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CCSoftInternshipProjectDocument, CDocument)

// Constructor / Destructor
// ----------------
CCSoftInternshipProjectDocument::CCSoftInternshipProjectDocument() noexcept
{
}

CCSoftInternshipProjectDocument::~CCSoftInternshipProjectDocument()
{
}

// Methods
// ----------------


// Overrides
// ----------------

BOOL CCSoftInternshipProjectDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}