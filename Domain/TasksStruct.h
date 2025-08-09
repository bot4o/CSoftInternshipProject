#pragma once
#include "pch.h"
#include "resource.h"
#include "DLLExport.h"

#define TASKS_NAME_LENGTH 64
#define TASKS_DESCRIPTION_LENGTH 128

/////////////////////////////////////////////////////////////////////////////
// TASKS

//TODO: enum za state
//enum State {
//    Finnished,
//};

/// <summary>Таблична структура от данни за таблица TASKS</summary>
struct TASKS
{
    // Constructor / Destructor
    // ----------------
public:
    TASKS()
    {
        SecureZeroMemory(this, sizeof(*this));
    }
    TASKS(const CString& strName, const CString& strDescription, const long& lProjectId, const short& sState, const short& sEffort)
    {
        SecureZeroMemory(this, sizeof(*this));
        this->lUpdateCounter = 0;
        _tcscpy_s(this->szName, strName);
        _tcscpy_s(this->szDescription, strDescription);
        this->lProjectId = lProjectId;
        this->lUserId = lUserId;
        this->sState = sState;
        this->sEffort = sEffort;
    }
    // Members
    // ----------------
    /// <summary>Уникален идентификатор за ред</summary>
    long lId;
    /// <summary>Брояч на промените по реда</summary>
    long lUpdateCounter;
    /// <summary>Име на задачата</summary>
    TCHAR szName[TASKS_NAME_LENGTH];
    /// <summary>Описание на задачата</summary>
    TCHAR szDescription[TASKS_DESCRIPTION_LENGTH];
    /// <summary>Идентификатор на проекта, към който е задачата</summary>
    long lProjectId;
    /// <summary>Идентификатор на потребителя, на когото е възложена задачата</summary>
    long lUserId;
    /// <summary>Състояние на задачата: 0 -> Изчакваща; 1 -> В процес; 2 -> Завършена</summary>
    short sState;
    /// <summary>Усилия по задачата</summary>
    short sEffort;
};
typedef CTypedPtrArray<CPtrArray, TASKS*> CTasksTypedPtrArray;