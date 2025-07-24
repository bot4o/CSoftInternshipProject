#pragma once

#include "Resource.h"  
#include "pch.h"  
#include "UsersStruct.h"  
#include "atldbcli.h"  
#include "UsersAccessor.h"

#include "DLLExport.h"

#define SQL_SELECT_ALL "SELECT * FROM %s WITH (HOLDLOCK, ROWLOCK)"
#define SQL_SELECT_BY_ID "SELECT * FROM %s WITH (ROWLOCK, UPDLOCK) WHERE ID = %d"
#define SQL_UPDATE_JOB_TITLE_BY_ID "UPDATE %s WITH (ROWLOCK, XLOCK) SET JOB_TITLE = N'%s' WHERE ID = %d"
#define SQL_INSERT "INSERT INTO %s WITH (ROWLOCK) (NAME, EMAIL, JOB_TITLE) VALUES ('%s', '%s', '%s')"
#define SQL_DELETE_BY_ID "DELETE FROM %s WITH (ROWLOCK, XLOCK) WHERE ID = %d"

/// <summary>Клас за работа с таблица USERS</summary>  
class DatabaseDLL_EXP CUsersTable
{

public:
    /// <summary>Извежда всички потребители в базата</summary>  
    bool SelectAll(CUsersTypedPtrArray& oUsersArray);
    /// <summary>Извежда потребител от базата според ID</summary>  
    bool SelectWhereID(const long lID, USERS& recUser);
    /// <summary>Променя длъжноста на потребител от базата според ID</summary>  
    bool UpdateWhereID(const long lID, const USERS& recUser);
    /// <summary>Вмъква нов потребител в базата</summary>  
    bool Insert(const USERS& recUser);
    /// <summary>Изтрива потребител от базата според ID</summary>  
    bool DeleteWhereID(const long lID);

    CUsersTable() {
        //TODO: 
    }
private:
    /// <summary>Командата</summary>  
    CCommand<CAccessor<CUsersAccessor>> m_oCommand;
    CString strUser = _T("USERS");
};
