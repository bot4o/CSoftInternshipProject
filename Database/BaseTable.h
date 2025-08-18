#pragma once
#include "pch.h"  
#include "Resource.h"  
#include "atldbcli.h"
#include "DBConnection.h"
#include "DLLExport.h"

#define IDENTITY_ACCESSOR_INDEX 0
#define DATA_ACCESSOR_INDEX 1
#define SQL_SELECT_ALL_NOLOCK _T("SELECT * FROM %s WITH (NOLOCK)")
#define SQL_SELECT_ALL _T("SELECT * FROM %s")
#define SQL_SELECT_BY_ID _T("SELECT * FROM %s WITH (ROWLOCK, UPDLOCK, HOLDLOCK) WHERE ID = %d")
#define SQL_SELECT_EMPTY _T("SELECT TOP 0 * FROM %s")

/////////////////////////////////////////////////////////////////////////////
// CBaseTable

/// <summary>Клас за работа с таблица</summary>  
template<typename TRecord, typename TAccessor>
class CBaseTable
{
    // Constructor / Destructor
    // ----------------
public:
    CBaseTable(const CString& strTableName) : m_strTable(strTableName) {}
    virtual ~CBaseTable() {}

    // Methods
    // ----------------
        /// <summary>Извежда всички потребители в базата</summary>  
    bool SelectAll(CTypedPtrArray<CPtrArray, TRecord*>& oRecordArray)
    {
        CDataSource& oDataSource = CDBConnection::GetDataSource();
        if (oDataSource.m_spInit == nullptr)
        {
            CDBConnection::OpenConnection();
        }

        HRESULT hResult = m_oSession.Open(oDataSource);
        if (FAILED(hResult))
        {
            CString msg;
            msg.Format(_T("Unable to open session. HRESULT: 0x%08X", hResult));
            AfxMessageBox(msg);
            return false;
        }

        CString strQuery;
        strQuery.Format(SQL_SELECT_ALL, m_strTable);

        hResult = m_oCommand.Open(m_oSession, strQuery);
        if (FAILED(hResult))
        {
            CString msg;
            msg.Format(_T("Unable to execute query. HRESULT: 0x%08X", hResult));
            AfxMessageBox(msg);
            m_oSession.Close();
            return false;
        }

        oRecordArray.RemoveAll();

        while ((hResult = m_oCommand.MoveNext()) == S_OK)
        {
            TRecord* pRecord = new TRecord(m_oCommand.GetRecord()); // <-- 
            oRecordArray.Add(pRecord);
        }

        if (hResult != DB_S_ENDOFROWSET)
        {
            CString msg;
            msg.Format(_T("Error reading data. HRESULT: %d", hResult));
            AfxMessageBox(msg);
            return false;
        }

        m_oCommand.Close();
        m_oSession.Close();
        return true;
    }
    /// <summary>Извежда потребител от базата според ID</summary>  
    bool SelectWhereID(const long lID, TRecord& oRecord)
    {
        CDataSource& oDataSource = CDBConnection::GetDataSource();

        HRESULT hResult = m_oSession.Open(oDataSource);
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X", hResult));
            AfxMessageBox(strMessage);
            return false;
        }

        hResult = m_oSession.StartTransaction();
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
            m_oSession.Close();
            return false;
        }

        CString strQuery;
        strQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);

        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Unable to execute command for the SQL Server database. HRESULT: 0x%08X", hResult));
            AfxMessageBox(strMessage);
            m_oSession.Abort();
            m_oSession.Close();
            return false;
        }

        hResult = m_oCommand.MoveFirst();
        if (hResult != S_OK)
        {
            CString strMessage;
            strMessage.Format(_T("%s oRecord with ID of %d was not found"), m_strTable, lID);
            AfxMessageBox(strMessage);
            m_oSession.Abort();
            m_oCommand.Close();
            m_oSession.Close();
            return false;
        }

        oRecord = m_oCommand.GetRecord(); // <--

        hResult = m_oSession.Commit();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSession.Abort();
            m_oSession.Close();
            return false;
        }

        m_oCommand.Close();
        m_oSession.Close();
        return true;
    }
    /// <summary>Променя длъжноста на потребител от базата според ID</summary>  
    bool UpdateWhereID(const long lID, TRecord& oRecord)
    {
        CDataSource& oDataSource = CDBConnection::GetDataSource();

        HRESULT hResult = m_oSession.Open(oDataSource);
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X", hResult));
            AfxMessageBox(strMessage);
            return false;
        }

        hResult = m_oSession.StartTransaction();
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
            m_oSession.Close();
            return false;
        }

        CString sUpdateQuery;
        sUpdateQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);

        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        hResult = m_oCommand.Open(m_oSession, sUpdateQuery, &oUpdateDBPropSet);
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to update %s oRecord. Error: %d", hResult, m_strTable));
            AfxMessageBox(strMessage);
            m_oSession.Abort();
            m_oSession.Close();
            return false;
        }
        //Ето тук
        //This move the "cursor" on the position of the first (and only) row of the query result
        hResult = m_oCommand.MoveNext();
        if (hResult == DB_S_ENDOFROWSET)
        {
            AfxMessageBox(_T("No %s found with the specified ID to update.", m_strTable));
            m_oSession.Abort();
            m_oCommand.Close();
            m_oSession.Close();
            return true;
        }

        TRecord& oDatabaseRecord = m_oCommand.GetRecord();
        if (oRecord.lUpdateCounter != oDatabaseRecord.lUpdateCounter)
        {
            AfxMessageBox(_T("Update counters do not match in the database"));
            m_oSession.Abort();
            m_oCommand.Close();
            m_oSession.Close();
            return false;
        }

        oDatabaseRecord = oRecord;
        oDatabaseRecord.lUpdateCounter += 1;
        oRecord.lUpdateCounter += 1;

        hResult = m_oCommand.SetData(DATA_ACCESSOR_INDEX);
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
            m_oSession.Abort();
            m_oCommand.Close();
            m_oSession.Close();
            return false;
        }

        hResult = m_oSession.Commit();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSession.Abort();
            m_oSession.Close();
            return false;
        }
        m_oCommand.Close();
        m_oSession.Close();
        return true;
    }
    /// <summary>Вмъква нов потребител в базата</summary>  
    bool Insert(TRecord& oRecord)
    {
        CDataSource& oDataSource = CDBConnection::GetDataSource();

        HRESULT hResult = m_oSession.Open(oDataSource);
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X", hResult));
            AfxMessageBox(strMessage);
            return false;
        }

        CString strQuery;
        strQuery.Format(SQL_SELECT_EMPTY, m_strTable);

        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to insert %s oRecord. Error: %d", m_strTable, hResult));
            AfxMessageBox(strMessage);
            m_oCommand.Close();
            m_oSession.Close();
            return false;
        }

        TRecord& oDatabaseRecord = m_oCommand.GetRecord();
        oDatabaseRecord = oRecord;

        hResult = m_oCommand.Insert(DATA_ACCESSOR_INDEX);
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
            m_oCommand.Close();
            m_oSession.Close();
            return false;
        }

        hResult = m_oCommand.MoveFirst();
        if (hResult != S_OK)
        {
            AfxMessageBox(_T("Unable to read inserted %s oRecord again from the SQL Server database. Error: %d", m_strTable, hResult));
            m_oCommand.Close();
            m_oSession.Close();
            return true;
        }
        oRecord.lId = oDatabaseRecord.lId;

        m_oCommand.Close();
        m_oSession.Close();
        return true;
    }
    /// <summary>Изтрива потребител от базата според ID</summary>  
    bool DeleteWhereID(const long lID)
    {
        CDataSource& oDataSource = CDBConnection::GetDataSource();

        HRESULT hResult = m_oSession.Open(oDataSource);
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X", hResult));
            AfxMessageBox(strMessage);
            return false;
        }

        hResult = m_oSession.StartTransaction();
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
            m_oSession.Close();
            return false;
        }

        CString strQuery;
        strQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);

        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to delete %s oRecord. Error: %d", m_strTable, hResult));
            AfxMessageBox(strMessage);
            m_oSession.Abort();
            m_oSession.Close();
            return false;
        }

        hResult = m_oCommand.MoveFirst();
        if (hResult != S_OK)
        {
            AfxMessageBox(_T("No %s oRecord found with the specified ID", m_strTable));
            m_oSession.Abort();
            m_oCommand.Close();
            m_oSession.Close();
            return true;
        }
        hResult = m_oCommand.Delete();
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Failed to delete the %s. oRecord", m_strTable));
            m_oSession.Abort();
            m_oCommand.Close();
            m_oSession.Close();
            return false;
        }

        hResult = m_oSession.Commit();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSession.Abort();
            m_oSession.Close();
            return false;
        }
        m_oCommand.Close();
        m_oSession.Close();
        return true;
    }

    // Members
    // ----------------
private:
    /// <summary>Помага да изпълним команди по сесията</summary>  
    CCommand<CAccessor<TAccessor>> m_oCommand;
    // <summary>От коя таблица ще се извличат данни в заявките</summary>
    CString m_strTable;
    /// <summary>Самата сесия</summary
    CSession m_oSession;
};