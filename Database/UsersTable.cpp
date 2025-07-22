#include "pch.h"
#include "atldbcli.h"
#include "UsersTable.h"

bool CUsersTable::SelectAll(CUsersTypedPtrArray& oUsersArray)
{
	HRESULT hrCom = CoInitialize(NULL);
	if (FAILED(hrCom))
	{
		AfxMessageBox(_T("Failed to initialize COM"));
		return false;
	}

	CDataSource oDataSource;
	CSession oSession;
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("."));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));  // Windows Authentication
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Internship_DB")); // your database name
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4)); // Prompt = Complete

	HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to connect to SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}
	// Open session
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to open session for the SQL Server database.Error: %d"), hResult);
		AfxMessageBox(msg);
		oDataSource.Close();
		return false;
	}

	// Конструираме заявката
	CString strQuery = _T(SELECT_ALL_USERS);
	// Изпълняваме командата
	hResult = m_oCommand.Open(oSession, strQuery);
	if (FAILED(hResult))
	{

		CString msg;
		msg.Format(_T("Unable to execute command for the SQL Server database.Error: %d"), hResult);
		AfxMessageBox(msg);
		oSession.Close();
		oDataSource.Close();

		return false;
	}
	// Прочитаме всички данни
	while ((hResult = m_oCommand.MoveNext()) == S_OK)
	{
		USERS* pUser = new USERS(m_oCommand.GetRecUser());
		if (pUser != NULL)
		{
			//*pUser = m_oCommand.m_recUser;
			oUsersArray.Add(pUser);
		}
	}
	if (hResult != DB_S_ENDOFROWSET)
	{
		CString msg;
		msg.Format(_T("Error reading data from the SQL Server database. Error: %d"), hResult);
		AfxMessageBox(msg);
		AfxMessageBox(_T(""), hResult);
		return false;
	}

	// Затваряме командата, сесията и връзката с базата данни. 
	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}
bool CUsersTable::SelectWhereID(const long lID, USERS& recUser)
{
	HRESULT hrCom = CoInitialize(NULL);
	if (FAILED(hrCom))
	{
		AfxMessageBox(_T("Failed to initialize COM"));
		return false;
	}

	CDataSource oDataSource;
	CSession oSession;
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("."));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Internship_DB"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to connect to SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}

	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}

	CString strQuery;
	strQuery.Format(_T(SELECT_ALL_USERS_BY_ID), lID);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	hResult = m_oCommand.Open(oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to execute command for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}
	
	hResult = m_oCommand.MoveFirst();
	if (hResult == S_OK)
	{
		recUser = m_oCommand.GetRecUser();
	}
	else
	{
		CString msg;
		msg.Format(_T("User with ID %d not found"), lID);
		AfxMessageBox(msg);
		m_oCommand.Close();
		oSession.Close();
		oDataSource.Close();

		return false;
	}

	//check
	if (recUser.lUpdateCounter == m_oCommand.GetRecUser().lUpdateCounter)
	{
        USERS& recUser = m_oCommand.GetRecUser();  
        recUser.lUpdateCounter += 1;
		hResult = m_oCommand.SetData(USERS_DATA_ACCESSOR_INDEX);
		if (FAILED(hResult))
		{
			AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
			oSession.Abort();

			m_oCommand.Close();
			oSession.Close();
			oDataSource.Close();

			return false;
		}
	}

	//commit transaction

	oSession.Commit();

	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}
bool CUsersTable::UpdateWhereID(const long lID, const USERS& recUser) {
	HRESULT hrCom = CoInitialize(NULL);
	if (FAILED(hrCom))
	{
		AfxMessageBox(_T("Failed to initialize COM"));
		return false;
	}

	CDataSource oDataSource;
	CSession oSession;
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("."));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Internship_DB"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to connect to SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}

	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}

	/*UPDATE USERS
		SET JOB_TITLE = N'Старши разработчик'
		WHERE EMAIL IN('petar@example.com', 'georgi@example.com')

		IF @@ERROR < > 0
		BEGIN
		PRINT 'Something went wrong!';
	ROLLBACK;
	END*/

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	CString strQuery;
	strQuery.Format(_T(UPDATE_USERS_JOB_TITLE_BY_ID), lID);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	hResult = m_oCommand.Open(oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Failed to update user record. Error: %d"), hResult);
		AfxMessageBox(msg);
		oSession.Abort();
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	//TODO: Check if any rows were affected


	//check
	if (recUser.lUpdateCounter == m_oCommand.GetRecUser().lUpdateCounter)
	{
		USERS& recUser = m_oCommand.GetRecUser();
		recUser.lUpdateCounter += 1;
		hResult = m_oCommand.SetData(USERS_DATA_ACCESSOR_INDEX);
		if (FAILED(hResult))
		{
			AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
			oSession.Abort();

			m_oCommand.Close();
			oSession.Close();
			oDataSource.Close();

			return false;
		}
	}

	oSession.Commit();
	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}
bool CUsersTable::Insert(const USERS& recUser) 
{
	HRESULT hrCom = CoInitialize(NULL);
	if (FAILED(hrCom))
	{
		AfxMessageBox(_T("Failed to initialize COM"));
		return false;
	}

	CDataSource oDataSource;
	CSession oSession;
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("."));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Internship_DB"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to connect to SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}

	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}

	/*INSERT INTO USERS(NAME, EMAIL, JOB_TITLE)
		VALUES
		(N'Иван Иванов', 'ivan@example.com', N'Ръководител'),*/

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	CString strQuery;
	strQuery.Format(_T(INSERT_USER), recUser.szName, recUser.szEmail, recUser.szJobTitle);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	hResult = m_oCommand.Open(oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Failed to insert user record. Error: %d"), hResult);
		AfxMessageBox(msg);
		m_oCommand.Close();
		oSession.Abort();
		oSession.Close();
		oDataSource.Close();
		return false;
	}
	oSession.Commit();

	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}
bool CUsersTable::DeleteWhereID(const long lID) {
	HRESULT hrCom = CoInitialize(NULL);
	if (FAILED(hrCom))
	{
		AfxMessageBox(_T("Failed to initialize COM"));
		return false;
	}

	CDataSource oDataSource;
	CSession oSession;
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("."));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Internship_DB"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to connect to SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}

	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(msg);
		return false;
	}

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	CString strQuery;
	strQuery.Format(_T(DELETE_USER_BY_ID), lID);

	hResult = m_oCommand.Open(oSession, strQuery);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Failed to delete user record. Error: %d"), hResult);
		AfxMessageBox(msg);
		oSession.Abort();
		oSession.Close();
		oDataSource.Close();
		return false;
	}
	//TODO: Check if any rows were affected


	oSession.Commit();

	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}

