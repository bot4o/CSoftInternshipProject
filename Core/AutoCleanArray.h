#pragma once

#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "framework.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoCleanArray

template<class TYPE>
class CAutoCleanArray : public CTypedPtrArray<CPtrArray, TYPE*>
{
    // Constructor / Destructor
    // ----------------
public:
    CAutoCleanArray() {}
    virtual ~CAutoCleanArray() {
        CleanArray();
    }
    void CleanArray()
    {
        for (int i = 0; i < this->GetSize(); i++)
        {
            delete this->GetAt(i);
        }
        this->RemoveAll();
    }
    
};