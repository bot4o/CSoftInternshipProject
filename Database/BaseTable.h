#pragma once
#include "pch.h"  
#include "Resource.h"  
#include "atldbcli.h"
#include "DBConnection.h"
#include "DLLExport.h"
#include "SessionManager.h"

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
        m_oSessionManager.OpenSession();
        CString strQuery;
        strQuery.Format(SQL_SELECT_ALL, m_strTable);
        m_oSessionManager.ExecuteQuery(strQuery);

        oRecordArray.RemoveAll();

        HRESULT hResult = S_OK;
        while ((hResult = m_oSessionManager.MoveNext()) == S_OK)
        {
            TRecord* pRecord = new TRecord(m_oSessionManager.GetRecord());
            oRecordArray.Add(pRecord);
        }

        if (hResult != DB_S_ENDOFROWSET)
        {
            CString msg;
            msg.Format(_T("Error reading data. HRESULT: %d", hResult));
            AfxMessageBox(msg);
            return false;
        }
        m_oSessionManager.CloseSession();
        return true;
    }
    /// <summary>Извежда потребител от базата според ID</summary>  
    bool SelectWhereID(const long lID, TRecord& oRecord)
    {
        m_oSessionManager.OpenSession();
        m_oSessionManager.BeginTransaction();

        CString strQuery;
        strQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);
        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        if (m_oSessionManager.ExecuteQuery(strQuery, &oUpdateDBPropSet))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to update %s oRecord. Error: %d", hResult, m_strTable));
            AfxMessageBox(strMessage);
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        HRESULT hResult = m_oSessionManager.MoveFirst();
        if (hResult != S_OK)
        {
            CString strMessage;
            strMessage.Format(_T("%s oRecord with ID of %d was not found"), m_strTable, lID);
            AfxMessageBox(strMessage);
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        oRecord = m_oSessionManager.GetRecord(); // <--

        hResult = m_oSessionManager.CommitTransaction();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        m_oSessionManager.CloseSession();
        return true;
    }
    /// <summary>Променя длъжноста на потребител от базата според ID</summary>  
    bool UpdateWhereID(const long lID, TRecord& oRecord)
    {
        m_oSessionManager.OpenSession();
        m_oSessionManager.BeginTransaction();

        CString sUpdateQuery;
        sUpdateQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);
        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        if (!m_oSessionManager.ExecuteQuery(sUpdateQuery, &oUpdateDBPropSet))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to update %s oRecord. Error: %d", hResult, m_strTable));
            AfxMessageBox(strMessage);
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }
        HRESULT hResult = m_oSessionManager.MoveNext();
        if (hResult == DB_S_ENDOFROWSET)
        {
            AfxMessageBox(_T("No %s found with the specified ID to update.", m_strTable));
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return true;
        }

        TRecord& oDatabaseRecord = m_oSessionManager.GetRecord();
        if (oRecord.lUpdateCounter != oDatabaseRecord.lUpdateCounter)
        {
            AfxMessageBox(_T("Update counters do not match in the database"));
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        oDatabaseRecord = oRecord;
        oDatabaseRecord.lUpdateCounter += 1;
        oRecord.lUpdateCounter += 1;

        hResult = m_oSessionManager.SetData(DATA_ACCESSOR_INDEX);
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        hResult = m_oSessionManager.CommitTransaction();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }
        m_oSessionManager.CloseSession();
        return true;
    }
    /// <summary>Вмъква нов потребител в базата</summary>  
    bool Insert(TRecord& oRecord)
    {
        m_oSessionManager.OpenSession();

        CString strQuery;
        strQuery.Format(SQL_SELECT_EMPTY, m_strTable);
        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        if (!m_oSessionManager.ExecuteQuery(strQuery, &oUpdateDBPropSet))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to update %s oRecord. Error: %d", hResult, m_strTable));
            AfxMessageBox(strMessage);
            m_oSessionManager.CloseSession();
            return false;
        }

        TRecord& oDatabaseRecord = m_oSessionManager.GetRecord();
        oDatabaseRecord = oRecord;

        HRESULT hResult = m_oSessionManager.InsertData(DATA_ACCESSOR_INDEX);
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
            m_oSessionManager.CloseSession();

            return false;
        }

        hResult = m_oSessionManager.MoveFirst();
        if (hResult != S_OK)
        {
            AfxMessageBox(_T("Unable to read inserted %s oRecord again from the SQL Server database. Error: %d", m_strTable, hResult));
            m_oSessionManager.CloseSession();
            return true;
        }
        oRecord.lId = oDatabaseRecord.lId;

        m_oSessionManager.CloseSession();
        return true;
    }
    /// <summary>Изтрива потребител от базата според ID</summary>  
    bool DeleteWhereID(const long lID)
    {
        m_oSessionManager.OpenSession();
        m_oSessionManager.BeginTransaction();

        CString strQuery;
        strQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);
        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        if (!m_oSessionManager.ExecuteQuery(strQuery, &oUpdateDBPropSet))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to delete %s oRecord. Error: %d", m_strTable, hResult));
            AfxMessageBox(strMessage);
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();

            return false;
        }

        HRESULT hResult = m_oSessionManager.MoveFirst();
        if (hResult != S_OK)
        {
            AfxMessageBox(_T("No %s oRecord found with the specified ID", m_strTable));
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return true;
        }
        hResult = m_oSessionManager.Delete();
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Failed to delete the %s. oRecord", m_strTable));
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        hResult = m_oSessionManager.CommitTransaction();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }
        m_oSessionManager.CloseSession();
        return true;
    }

    // Members
    // ----------------
private:
    CSessionManager<TRecord, TAccessor> m_oSessionManager; 
    // <summary>От коя таблица ще се извличат данни в заявките</summary>
    CString m_strTable;
};