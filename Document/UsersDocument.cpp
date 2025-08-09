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
bool CUsersDocument::AddUser(USERS& oRecUser)
{
	if (!CUsersAppService().Insert(oRecUser))
	{
		AfxMessageBox(_T("Error at the CUsersAppService().Insert() in the document layer"));
		return false;
	}
	m_oUsersArray.Add(new USERS(oRecUser));
	UpdateAllViews(nullptr, (long)Modes::InsertMode, (CObject*)&oRecUser);
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

bool CUsersDocument::UpdateUser(const long lID, USERS& oRecUser)
{
	if (!CUsersAppService().UpdateWhereID(lID, oRecUser))
	{
		AfxMessageBox(_T("Error at the CUsersAppService().UpdateWhereID() in the document layer"));

		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::UpdateMode, (CObject*)&oRecUser);
	return true;
}

bool CUsersDocument::DeleteUser(const long lID)
{
	if (!CUsersAppService().DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the CUsersAppService().DeleteWhereID() in the document layer"));

		return false;
	}
	for (int i = 0; i < m_oUsersArray.GetSize(); i++)
	{
		if (lID == m_oUsersArray[i]->lId) 
		{
			m_oUsersArray.RemoveAt(i);
			break;
		}
	}
	UpdateAllViews(nullptr, (long)Modes::DeleteMode, (CObject*)&m_oUsersArray[0]);
	return true;
}

CUsersTypedPtrArray& CUsersDocument::GetUsers() 
{
	return m_oUsersArray;
}

static void UpdateAllViews(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL)
{

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