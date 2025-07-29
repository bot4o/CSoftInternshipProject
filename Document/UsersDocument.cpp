#include "pch.h"
#include "UsersDocument.h"


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
bool CUsersDocument::AddUser(USERS m_oRecUser)
{
	m_oUsersService.Insert(m_oRecUser);
	return true;
}
bool CUsersDocument::LoadAllUsers()
{
	m_oUsersService.SelectAll(m_oUsersArray);
	return true;
}
bool CUsersDocument::UpdateUser(long m_lID, USERS& m_oRecUser)
{
	m_oUsersService.UpdateWhereID(m_lID, m_oRecUser);
	return true;
}
bool CUsersDocument::DeleteUser(long m_lID)
{
	m_oUsersService.DeleteWhereID(m_lID);
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

