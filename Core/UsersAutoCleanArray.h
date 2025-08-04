#pragma on

#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "framework.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoCleanArray

template<class Array, class TYPE>
class CAutoCleanArray : CTypedPtrArray<CPtrArray, TYPE>
{
    // Constructor / Destructor
    // ----------------
public:
    CAutoCleanArray();
    virtual ~CAutoCleanArray() {
        for (int i = 0; i < GetSize(); i++) 
        {
            delete GetAt(i);
        }
        RemoveAll();
    }
};