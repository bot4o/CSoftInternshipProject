#pragma once

#include "resource.h"
#include "pch.h"

#define USERS_NAME_LENGTH 64
#define USERS_EMAIL_LENGTH 64
#define USERS_JOB_TITLE_LENGTH 32

/// <summary>Таблична структура от данни за таблица USERS</summary>
struct USERS
{
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

    USERS()
    {
        SecureZeroMemory(this, sizeof(*this));
    }
};

typedef CTypedPtrArray<CPtrArray, USERS*> CUsersTypedPtrArray;