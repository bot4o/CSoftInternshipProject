#include "pch.h"
#include "atldbcli.h"
#include "DBConnection.h"

/////////////////////////////////////////////////////////////////////////////
// CDBConnection
CDataSource CDBConnection::m_oDataSource;

// Constructor / Destructor
// ----------------
CDBConnection::CDBConnection()
{
}

CDBConnection::~CDBConnection()
{
}

// Methods
// ----------------
bool CDBConnection::OpenConnection()
{
	HRESULT hr = ::CoInitialize(NULL);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to initialize COM library."));
		return FALSE;
	} 

	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("."));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Internship_DB"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	HRESULT hResult = m_oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to connect to SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}
	return true;
}
CDataSource& CDBConnection::GetDataSource()
{
	return m_oDataSource;
}
CDBPropSet CDBConnection::GetDbPropSet() {
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
	return oUpdateDBPropSet;
}



