#pragma once

#include "Resource.h"  
#include "pch.h"  
#include "atldbcli.h"
#include "TasksStruct.h"  
#include "TasksAccessor.h"
#include "DBConnection.h"
#include "DLLExport.h"

#define TABLE_TASKS _T("TASKS")
#define SQL_SELECT_ALL_NOLOCK _T("SELECT * FROM %s WITH (NOLOCK)")
#define SQL_SELECT_ALL _T("SELECT * FROM %s")
#define SQL_SELECT_BY_ID _T("SELECT * FROM %s WITH (ROWLOCK, UPDLOCK, HOLDLOCK) WHERE ID = %d")
#define SQL_SELECT_EMPTY _T("SELECT TOP 0 * FROM %s")


/////////////////////////////////////////////////////////////////////////////
// CTasksTable

/// <summary>Клас за работа с таблица TASKS</summary>  
class DatabaseDLL_EXP CTasksTable
{

    // Constructor / Destructor
    // ----------------
public:
    CTasksTable();
    virtual ~CTasksTable();

    // Methods
    // ----------------
        /// <summary>Извежда всички потребители в базата</summary>  
    bool SelectAll(CTasksTypedPtrArray& oTasksArray);
    /// <summary>Извежда потребител от базата според ID</summary>  
    bool SelectWhereID(const long lID, TASKS& recTask);
    /// <summary>Променя длъжноста на потребител от базата според ID</summary>  
    bool UpdateWhereID(const long lID, TASKS& recTask);
    /// <summary>Вмъква нов потребител в базата</summary>  
    bool Insert(TASKS& recTask);
    /// <summary>Изтрива потребител от базата според ID</summary>  
    bool DeleteWhereID(const long lID);

    // Members
    // ----------------
private:
    /// <summary>Помага да изпълним команди по сесията</summary>  
    CCommand<CAccessor<CTasksAccessor>> m_oCommand;
    // <summary>От коя таблица ще се извличат данни в заявките</summary>
    CString strTable = TABLE_TASKS;
    /// <summary>Самата сесия</summary
    CSession m_oSession;
};
