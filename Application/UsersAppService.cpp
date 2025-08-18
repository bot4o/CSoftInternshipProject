#include "pch.h"
#include "UsersAppService.h"
#include "UsersAccessor.h"
#include "BaseTable.h"


#define TABLE_NAME _T("USERS")
// Constructor / Destructor
// ----------------
CUsersAppService::CUsersAppService() 
{

}
CUsersAppService::~CUsersAppService()
{

}

// Methods
// ----------------

/// <summary>Извежда всички потребители в базата</summary>  
bool CUsersAppService::SelectAll(CUsersTypedPtrArray& oUsersArray)
{
	if (!CBaseTable<USERS, CUsersAccessor>(TABLE_NAME).SelectAll(oUsersArray))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда потребител от базата според ID</summary>  
bool CUsersAppService::SelectWhereID(const long lID, USERS& recUser)
{
	if (!CBaseTable<USERS, CUsersAccessor>(TABLE_NAME).SelectWhereID(lID, recUser))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.SelectWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CUsersAppService::UpdateWhereID(const long lID, USERS& recUser)
{
	if (!CBaseTable<USERS, CUsersAccessor>(TABLE_NAME).UpdateWhereID(lID, recUser))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.UpdateWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Вмъква нов потребител в базата</summary>  
bool CUsersAppService::Insert(USERS& recUser)
{
	if (!CBaseTable<USERS, CUsersAccessor>(TABLE_NAME).Insert(recUser))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.Insert() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Изтрива потребител от базата според ID</summary>  
bool CUsersAppService::DeleteWhereID(const long lID)
{
	if (!CBaseTable<USERS, CUsersAccessor>(TABLE_NAME).DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.DeleteWhereID() in the application layer"));
		return false;
	}
	return true;
}