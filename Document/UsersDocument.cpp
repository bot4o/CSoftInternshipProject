#include "pch.h"
#include "UsersDocument.h"
#include "UsersAppService.h"
#include "DialogModes.h"


/////////////////////////////////////////////////////////////////////////////
// CUsersDocument
// 

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CUsersDocument, CDocument)

// Constructor / Destructor
// ----------------
CUsersDocument::CUsersDocument()
{
}

CUsersDocument::~CUsersDocument()
{
}

// Methods
// ----------------
//CRUD
bool CUsersDocument::AddUser(const USERS& oRecUser)
{
	if (!CUsersAppService().Insert(oRecUser))
	{
		AfxMessageBox(_T("Error at the CUsersAppService().Insert() in the document layer"));
		return false;
	}
	UpdateAllViews(nullptr, InsertMode, (CObject*)&oRecUser);
	return true;
}
bool CUsersDocument::LoadAllUsers()
{
	if (!CUsersAppService().SelectAll(m_oUsersArray))
	{
		AfxMessageBox(_T("Error at the CUsersAppService().SelectAll() in the document layer"));

		return false;
	}
	return true;
}
bool CUsersDocument::UpdateUser(const long m_lID, USERS& oRecUser)
{
	if (!CUsersAppService().UpdateWhereID(m_lID, oRecUser))
	{
		AfxMessageBox(_T("Error at the CUsersAppService().UpdateWhereID() in the document layer"));

		return false;
	}
	UpdateAllViews(nullptr, UpdateMode, (CObject*)&oRecUser);
	return true;
}
bool CUsersDocument::DeleteUser(const long m_lID)
{
	if (!CUsersAppService().DeleteWhereID(m_lID))
	{
		AfxMessageBox(_T("Error at the CUsersAppService().DeleteWhereID() in the document layer"));

		return false;
	}
	USERS oRecUser = USERS();
	UpdateAllViews(nullptr, DeleteMode, (CObject*)&oRecUser);
	return true;
}
CUsersTypedPtrArray& CUsersDocument::GetUsers() 
{
	return m_oUsersArray;
}
void UpdateAllViews(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL)
{
	//TODO how to use UpdateAllViews to trigger CUsersView.OnUpdate() and .OnInitialUpdate()
}

// Overrides
// ----------------
BOOL CUsersDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!LoadAllUsers()) {
		AfxMessageBox(_T("Cound't initialize all users into this document"));
		return FALSE;
	}
	return TRUE;
}