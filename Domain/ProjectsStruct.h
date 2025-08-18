#pragma once
#include "pch.h"
#include "resource.h"
#include "DLLExport.h"
#include "AutoCleanArray.h"

#define PROJECTS_NAME_LENGTH 64
#define PROJECTS_DESCRIPTION_LENGTH 128

/////////////////////////////////////////////////////////////////////////////
// PROJECTS

/// <summary>Таблична структура от данни за таблица PROJECTS</summary>
struct PROJECTS
{
    // Constructor / Destructor
    // ----------------
public:
    PROJECTS()
    {
        SecureZeroMemory(this, sizeof(*this));
    }
    PROJECTS(const CString& strName, const CString& strDescription, const long& lProjectManagerId, const bool& sState, const short& sTotalEffort)
    {
        SecureZeroMemory(this, sizeof(*this));
        this->lUpdateCounter = 0;
        _tcscpy_s(this->szName, strName);
        _tcscpy_s(this->szDescription, strDescription);
        this->lProjectManagerId = lProjectManagerId;
        this->sState = sState;
        this->sTotalEffort = sTotalEffort;
    }
    // Members
    // ----------------
    /// <summary>Уникален идентификатор за ред</summary>
    long lId;
    /// <summary>Брояч на промените по реда</summary>
    long lUpdateCounter;
    /// <summary>Име на проекта</summary>
    TCHAR szName[PROJECTS_NAME_LENGTH];
    /// <summary>Описание на проекта</summary>
    TCHAR szDescription[PROJECTS_DESCRIPTION_LENGTH];
    /// <summary>Идентификатор на мениджър на проекта</summary>
    long lProjectManagerId;
    /// <summary>Състояние на проекта: 0 -> Активен; 1 -> Приключен;</summary>
    short sState;
    /// <summary>  Общо вложени усилия в проект</summary>
    short sTotalEffort;
};
typedef CAutoCleanArray<PROJECTS> CProjectsTypedPtrArray;