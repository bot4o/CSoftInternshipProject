#pragma once
#include "pch.h"
#include "Resource.h"  
#include "atldbcli.h"  


class CDBConnection
{
public:
	// Constructors
	// ----------------
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