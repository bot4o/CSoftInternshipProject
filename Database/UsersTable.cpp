#include "pch.h"
#include "atldbcli.h"
#include "UsersTable.h"

bool CUsersTable::OpenConnection(CDataSource oDataSource, CSession oSession)
{
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
}

bool CUsersTable::SelectAll(CUsersTypedPtrArray& oUsersArray)
{
	CDataSource oDataSource;
	CSession oSession;

	if (!OpenConnection(oDataSource, oSession)) {
		return false;
	}

	// Конструираме заявката
	CString strQuery;
	strQuery.Format(SQL_SELECT_ALL, strTable);
	// Изпълняваме командата
	HRESULT hResult = m_oCommand.Open(oSession, strQuery);
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
	CDataSource oDataSource;
	CSession oSession;

	if (!OpenConnection(oDataSource, oSession)) {
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_BY_ID, strTable, lID);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	HRESULT hResult = m_oCommand.Open(oSession, strQuery, &oUpdateDBPropSet);
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
bool CUsersTable::UpdateWhereID(const long lID, const USERS& recUser) 
{
	CDataSource oDataSource;
	CSession oSession;

	if (!OpenConnection(oDataSource, oSession)) {
		return false;
	}

	HRESULT hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	CString sUpdateQuery;
	sUpdateQuery.Format(SQL_SELECT_BY_ID, strTable, lID);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	hResult = m_oCommand.Open(oSession, sUpdateQuery, &oUpdateDBPropSet);
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

	hResult = m_oCommand.MoveFirst();
	if (hResult == S_OK)
	{
		//?
		m_oCommand.GetRecUser() = recUser;
	}

	//if (m_oCommand.MoveFirst() == S_OK) {
	//	// Обновяваме JOB_TITLE полето
	//	_tcscpy_s(oCmd.m_szJobTitle, sNewJobTitle);

	//	// Изпращаме промените към базата
	//	hr = oCmd.SetData(UPDATE_CHANGED);

	//	if (FAILED(hr)) {
	//		AfxMessageBox(_T("Грешка при запис на промените (UPDATE)."));
	//		return false;
	//	}
	//}
	else {
		AfxMessageBox(_T("Потребителят не е намерен."));
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

	oSession.Commit();
	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}
bool CUsersTable::Insert(const USERS& recUser) 
{
	CDataSource oDataSource;
	CSession oSession;

	if (!OpenConnection(oDataSource, oSession)) {
		return false;
	}

	HRESULT hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_EMPTY, strTable);

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

	m_oCommand.Insert();
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Failed to prepare new row for insert. Error: %d"), hResult);
		AfxMessageBox(msg);
		m_oCommand.Close();
		oSession.Abort();
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	_tcscpy_s(m_oCommand.GetRecUser().szName, USERS_NAME_LENGTH, recUser.szName);
	_tcscpy_s(m_oCommand.GetRecUser().szEmail, USERS_NAME_LENGTH, recUser.szEmail);
	_tcscpy_s(m_oCommand.GetRecUser().szJobTitle, USERS_NAME_LENGTH, recUser.szJobTitle);

	hResult = m_oCommand.SetData();
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Failed to set data for new user record. Error: %d"), hResult);
		AfxMessageBox(msg);
		m_oCommand.Close();
		oSession.Abort();
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	hResult = m_oCommand.Update();
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Failed to update database with new user record. Error: %d"), hResult);
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
bool CUsersTable::DeleteWhereID(const long lID) 
{
	CDataSource oDataSource;
	CSession oSession;

	if (!OpenConnection(oDataSource, oSession)) {
		return false;
	}

	HRESULT hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		oSession.Close();
		oDataSource.Close();
		return false;
	}
	CString strQuery;
	strQuery.Format(SQL_SELECT_BY_ID, strTable,  lID);

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

	hResult = m_oCommand.MoveNext();
	if (hResult == S_OK) 
	{
		m_oCommand.Delete();
		m_oCommand.Update();
	}
	else 
	{
		AfxMessageBox(_T("No user found with the specified ID to delete."));
		m_oCommand.Close();
		oSession.Abort();
		oSession.Close();
		oDataSource.Close();
		return true;

	}
	oSession.Commit();

	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}

