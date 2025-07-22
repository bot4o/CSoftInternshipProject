#pragma once

#include "Resource.h"  
#include "pch.h"  
#include "Users.h"  
#include "atldbcli.h"  
#include "UsersAccessor.h"

#include "DLLExport.h"

/// <summary>Клас за работа с таблица CUSTOMERS</summary>  
class DomainDLL_EXP CUsersTable
{
public:
    /// <summary> Selektirane </summary>  
    bool LoadAllUsers(CUsersTypedPtrArray& oUsersArray);
    /// <summary> ... </summary>  
    bool UpdateUser(USERS* pUser);
    //...  
private:
    /// <summary> ... </summary>  
    CCommand<CAccessor<CUsersAccessor>> m_oCommand;
};
