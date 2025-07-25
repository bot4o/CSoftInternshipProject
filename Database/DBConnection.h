#pragma once
#include "pch.h"
#include "Resource.h"  
#include "atldbcli.h"  

/////////////////////////////////////////////////////////////////////////////
// CDBConnection

class CDBConnection
{
// Constants
// ----------------


// Constructors
// ----------------
public:
	CDBConnection();
	virtual ~CDBConnection();


// Methods
// ----------------
	static bool OpenConnection();	
	static CDataSource GetDataSource();

// Overrides
// ----------------


// Members
// ----------------
	static CDataSource m_oDataSource;
};