#pragma once

#include "Resource.h"  
#include "pch.h"  
#include "Users.h"  
#include "atldbcli.h"  
#include "UsersAccessor.h"

#include "DLLExport.h"

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
