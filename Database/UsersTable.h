#pragma once

#include "Resource.h"  
#include "pch.h"  
#include "UsersStruct.h"  
#include "atldbcli.h"  
#include "UsersAccessor.h"
#include "DBConnection.h"

#include "DLLExport.h"

#define SQL_SELECT_ALL _T("SELECT * FROM %s WITH (NOLOCK)")
//#define SQL_SELECT_ALL _T("SELECT * FROM %s WITH ")

#define SQL_SELECT_BY_ID _T("SELECT * FROM %s WITH (ROWLOCK, UPDLOCK) WHERE ID = %d")
#define SQL_SELECT_EMPTY _T("SELECT * FROM %s WHERE 1 = 0")

/// <summary>Клас за работа с таблица USERS</summary>  
class DatabaseDLL_EXP CUsersTable
{
public:

    /// <summary>Извежда всички потребители в базата</summary>  
    bool SelectAll(CUsersTypedPtrArray& oUsersArray);
    /// <summary>Извежда потребител от базата според ID</summary>  
    bool SelectWhereID(const long lID, USERS& recUser);
    /// <summary>Променя длъжноста на потребител от базата според ID</summary>  
    bool UpdateWhereID(const long lID, USERS& recUser);
    /// <summary>Вмъква нов потребител в базата</summary>  
    bool Insert(const USERS& recUser);
    /// <summary>Изтрива потребител от базата според ID</summary>  
    bool DeleteWhereID(const long lID);

    

    CUsersTable() {
        //TODO: 
    }
private:
    
    /// <summary>
    ///     Отваря връзка с базата данни.
    /// </summary>
    /// <param name="oDataSource">Връзката със самата база данни</param>
    /// <param name="oSession">Сесия/Заявака за изпълнение на команди</param>
    /// <summary>Помага да изпълним команди по сесията</summary>  
    CCommand<CAccessor<CUsersAccessor>> m_oCommand;
    // <summary>От коя таблица ще се извличат данни в заявките</summary>
    CString strTable = _T("USERS");

    CSession m_oSession;
};
