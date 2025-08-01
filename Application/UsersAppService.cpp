#include "pch.h"
#include "UsersAppService.h"
#include "UsersTable.h"

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
	if (!CUsersTable().SelectAll(oUsersArray))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}
/// <summary>Извежда потребител от базата според ID</summary>  
bool CUsersAppService::SelectWhereID(const long lID, USERS& recUser)
{
	if (!CUsersTable().SelectWhereID(lID, recUser))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.SelectWhereID() in the application layer"));
		return false;
	}
	return true;
}
/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CUsersAppService::UpdateWhereID(const long lID, USERS& recUser)
{
	if (!CUsersTable().UpdateWhereID(lID, recUser))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.UpdateWhereID() in the application layer"));
		return false;
	}
	return true;
}
/// <summary>Вмъква нов потребител в базата</summary>  
bool CUsersAppService::Insert(const USERS& recUser)
{
	if (!CUsersTable().Insert(recUser))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.Insert() in the application layer"));
		return false;
	}
	return true;
}
/// <summary>Изтрива потребител от базата според ID</summary>  
bool CUsersAppService::DeleteWhereID(const long lID)
{
	if (!CUsersTable().DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.DeleteWhereID() in the application layer"));
		return false;
	}
	return true;
}