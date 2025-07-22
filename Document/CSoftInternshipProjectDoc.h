#pragma once
#include "DLLExport.h"

/////////////////////////////////////////////////////////////////////////////
// CCSoftInternshipProjectDocument
class DocumentDLL_EXP CCSoftInternshipProjectDocument : public CDocument
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CCSoftInternshipProjectDocument)

	// Constructor / Destructor
	// ----------------
public:
	CCSoftInternshipProjectDocument() noexcept;
	virtual ~CCSoftInternshipProjectDocument();

	// Methods
	// ----------------


	// Overrides
	// ----------------
public:
	BOOL OnNewDocument() override;

	// Members
	// ----------------
};
