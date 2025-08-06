#pragma once
#include "Resource.h"
#include "pch.h"
#include "TasksStruct.h"
#include "atldbcli.h"

#define TASKS_ACCESSORS_COUNT 2 //ID and all others
#define TASKS_IDENTITY_ACCESSOR_INDEX 0 //ID index
#define TASKS_DATA_ACCESSOR_INDEX 1 //All others first index?
#define TASKS_COLUMN_ENTRY_ID 1 // Column index
#define TASKS_COLUMN_ENTRY_UPDATE_COUNTER 2  //---//---
#define TASKS_COLUMN_ENTRY_NAME 3 
#define TASKS_COLUMN_ENTRY_DESCRIPTION 4
#define TASKS_COLUMN_ENTRY_PROJECT_ID 5 
#define TASKS_COLUMN_ENTRY_USER_ID 6
#define TASKS_COLUMN_ENTRY_STATE 7
#define TASKS_COLUMN_ENTRY_EFFORT 8

/////////////////////////////////////////////////////////////////////////////
// CTasksAccessor

/// <summary>Клас за достъп до таблицата TASKS.</summary>  
class CTasksAccessor
{
    // Methods
    // ----------------
public:
    /// <summary>GET method за достъп до m_recTask.</summary>  
    TASKS& GetRecord()
    {
        return m_recTask;
    }
    // Macros
    // ----------------
protected:
    /// <summary>Макрос дефиниращ публичен достъп до аксесор за определен тип клас.</summary>  
    BEGIN_ACCESSOR_MAP(CTasksAccessor, TASKS_ACCESSORS_COUNT)
        BEGIN_ACCESSOR(TASKS_IDENTITY_ACCESSOR_INDEX, true)
        COLUMN_ENTRY(TASKS_COLUMN_ENTRY_ID, m_recTask.lId)
        END_ACCESSOR()

        BEGIN_ACCESSOR(TASKS_DATA_ACCESSOR_INDEX, true)
        COLUMN_ENTRY(TASKS_COLUMN_ENTRY_UPDATE_COUNTER, m_recTask.lUpdateCounter)
        COLUMN_ENTRY(TASKS_COLUMN_ENTRY_NAME, m_recTask.szName)
        COLUMN_ENTRY(TASKS_COLUMN_ENTRY_DESCRIPTION, m_recTask.szDescription)
        COLUMN_ENTRY(TASKS_COLUMN_ENTRY_PROJECT_ID, m_recTask.lProjectId)
        COLUMN_ENTRY(TASKS_COLUMN_ENTRY_USER_ID, m_recTask.lUserId)
        COLUMN_ENTRY(TASKS_COLUMN_ENTRY_STATE, m_recTask.sState)
        COLUMN_ENTRY(TASKS_COLUMN_ENTRY_EFFORT, m_recTask.sEffort)
        END_ACCESSOR()
    END_ACCESSOR_MAP()

    // Members
    // ----------------
protected:
    /// <summary>TASK проментлива за заявките</summary>  
    TASKS m_recTask;
};
