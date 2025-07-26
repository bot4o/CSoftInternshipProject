#include "pch.h"
#include "UsersAppService.h"

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
	if (!m_oUsersTable.SelectAll(oUsersArray))
	{
		AfxMessageBox(_T("Error at the m_oUsersTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}
/// <summary>Извежда потребител от базата според ID</summary>  
bool CUsersAppService::SelectWhereID(const long lID, USERS& recUser)
{
	return true;
}
/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CUsersAppService::UpdateWhereID(const long lID, USERS& recUser)
{
	return true;
}
/// <summary>Вмъква нов потребител в базата</summary>  
bool CUsersAppService::Insert(const USERS& recUser)
{
	return true;
}
/// <summary>Изтрива потребител от базата според ID</summary>  
bool CUsersAppService::DeleteWhereID(const long lID)
{
	return true;
}