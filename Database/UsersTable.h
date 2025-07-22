#pragma once

#include "Resource.h"  
#include "pch.h"  
#include "Users.h"  
#include "atldbcli.h"  
#include "UsersAccessor.h"

#include "DLLExport.h"

#define SELECT_ALL_USERS "SELECT * FROM USERS"
#define SELECT_ALL_USERS_BY_ID "SELECT * FROM USERS WHERE ID = %d"
#define UPDATE_USERS_JOB_TITLE_BY_ID "UPDATE USERS SET JOB_TITLE = N'Старши разработчик' WHERE ID = %d"
#define INSERT_USER "INSERT INTO USERS (NAME, EMAIL, JOB_TITLE) VALUES ('%s', '%s', '%s')"
#define DELETE_USER_BY_ID "DELETE FROM USERS WHERE ID = %d"

/// <summary>Клас за работа с таблица CUSTOMERS</summary>  
class DatabaseDLL_EXP CUsersTable
{

public:
    /// <summary> Selektirane </summary>  
    bool SelectAll(CUsersTypedPtrArray& oUsersArray);
    bool SelectWhereID(const long lID, USERS& recUser);
    bool UpdateWhereID(const long lID, const USERS& recUser);
    bool Insert(const USERS& recUser);
    bool DeleteWhereID(const long lID);
    //...  
private:
    /// <summary> ... </summary>  
    CCommand<CAccessor<CUsersAccessor>> m_oCommand;
};
