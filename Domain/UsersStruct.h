#pragma once
#include "pch.h"
#include "resource.h"
#include "DLLExport.h"

#define USERS_NAME_LENGTH 64
#define USERS_EMAIL_LENGTH 64
#define USERS_JOB_TITLE_LENGTH 32

/////////////////////////////////////////////////////////////////////////////
// USERS

/// <summary>Таблична структура от данни за таблица USERS</summary>
struct USERS
{
// Constructor / Destructor
// ----------------
public:
    USERS()
    {
        SecureZeroMemory(this, sizeof(*this));
    }
    USERS(const CString& name, const CString& email, const CString& job)
    {
        SecureZeroMemory(this, sizeof(*this));
        this->lUpdateCounter = 0;
        _tcscpy_s(szName, name);
        _tcscpy_s(szEmail, email);
        _tcscpy_s(szJobTitle, job);
    }
// Members
// ----------------
    /// <summary>Уникален идентификатор за ред</summary>
    long lId;
    /// <summary>Versioning</summary>
    long lUpdateCounter;
    /// <summary>Име на потребителя</summary>
    TCHAR szName[USERS_NAME_LENGTH];
    /// <summary>Имейл на потребителя</summary>
    TCHAR szEmail[USERS_EMAIL_LENGTH];
    /// <summary>Длъжност на потребителя</summary>
    TCHAR szJobTitle[USERS_JOB_TITLE_LENGTH];
};
typedef CTypedPtrArray<CPtrArray, USERS*> CUsersTypedPtrArray;