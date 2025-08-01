#pragma once
#include "pch.h"
#include "Resource.h"  
#include "atldbcli.h"  
#include "DLLExport.h"


/////////////////////////////////////////////////////////////////////////////
// CDBConnection

class DatabaseDLL_EXP CDBConnection
{

// Constructors
// ----------------
public:
	CDBConnection();
	virtual ~CDBConnection();

// Methods
// ----------------
	static bool OpenConnection();	
	static CDataSource& GetDataSource();
	static CDBPropSet GetDbPropSet();

// Members
// ----------------
	static CDataSource m_oDataSource;
};