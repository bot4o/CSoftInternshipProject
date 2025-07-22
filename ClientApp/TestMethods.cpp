#include "pch.h"
#include "TestMethods.h"

//#include "AutoCleanCTypedPtrArray.h"

void TestMethods::ExampleCTypedPtrArray(CUsersTypedPtrArray& oUsersPtrArray)
{
    USERS* pUser1 = new USERS(_T("Иван"), _T("ivan@example.com"), _T("Разработчик"));
    USERS* pUser2 = new USERS(_T("Петър"), _T("petar@example.com"), _T("Ръководител"));
    USERS* pUser3 = new USERS(_T("Георgi"), _T("georgi@example.com"), _T("Разработчик"));

    oUsersPtrArray.Add(pUser1);
    oUsersPtrArray.Add(pUser2);
    oUsersPtrArray.Add(pUser3);
}
