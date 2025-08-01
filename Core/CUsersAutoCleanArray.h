#pragma on

#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "framework.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersAutoCleanArray

template<class Array, class TYPE>
class CUsersAutoCleanArray : CTypedPtrArray<CPtrArray, TYPE>
{

    // Constants
    // ----------------


    // Constructor / Destructor
    // ----------------
public:
    CUsersAutoCleanArray();
    virtual ~CUsersAutoCleanArray() {
        for (int i = 0; i < GetSize(); i++) {
            delete GetAt(i);
        }
        RemoveAll();
    }

    // Methods
    // ----------------


    // Overrides
    // ----------------


    // Members
    // ----------------


};