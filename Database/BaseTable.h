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
    CBaseTable(CSessionManager* oSessionManager /*nullptr*/, const CString& strTableName) : m_strTable(strTableName), m_oSessionManager(*oSessionManager) {
        if (oSessionManager == nullptr)
        {
            m_oSessionManager.OpenSession();
        }
    }
    virtual ~CBaseTable() {}

    // Methods
    // ----------------
    /// <summary>Извежда всички потребители в базата</summary>  
    bool SelectAll(CTypedPtrArray<CPtrArray, TRecord*>& oRecordArray)
    {
        CString strQuery;
        strQuery.Format(SQL_SELECT_ALL, m_strTable);
        if (!m_oSessionManager.ExecuteQuery(strQuery, m_oCommand))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to update %s oRecord. Error: %d", hResult, m_strTable));
            AfxMessageBox(strMessage);
            m_oSessionManager.CloseSession();
            return false;
        }

        oRecordArray.RemoveAll();

        HRESULT hResult = S_OK;
        while ((hResult = m_oCommand.MoveNext()) == S_OK)
        {
            TRecord* pRecord = new TRecord(m_oCommand.GetRecord());
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
        return true;
    }
    /// <summary>Извежда потребител от базата според ID</summary>  
    bool SelectWhereID(const long lID, TRecord& oRecord)
    {
        HRESULT hResult = m_oSessionManager.BeginTransaction();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSessionManager.CloseSession();
            return false;
        }

        CString strQuery;
        strQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);
        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        if (!m_oSessionManager.ExecuteQuery(strQuery, m_oCommand, &oUpdateDBPropSet))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to update %s oRecord. Error: %d", hResult, m_strTable));
            AfxMessageBox(strMessage);
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        hResult = m_oCommand.MoveFirst();
        if (hResult != S_OK)
        {
            CString strMessage;
            strMessage.Format(_T("%s oRecord with ID of %d was not found"), m_strTable, lID);
            AfxMessageBox(strMessage);
            m_oCommand.Close();
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        oRecord = m_oCommand.GetRecord();

        hResult = m_oSessionManager.CommitTransaction();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oCommand.Close();
            m_oSessionManager.CloseSession();
            return false;
        }
        m_oCommand.Close();
        return true;
    }
    /// <summary>Променя длъжноста на потребител от базата според ID</summary>  
    bool UpdateWhereID(const long lID, TRecord& oRecord)
    {
        CString sUpdateQuery;
        sUpdateQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);
        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        HRESULT hResult = m_oSessionManager.BeginTransaction();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSessionManager.CloseSession();
            return false;
        }

        if (!m_oSessionManager.ExecuteQuery(sUpdateQuery, m_oCommand,  &oUpdateDBPropSet))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to update %s oRecord. Error: %d", hResult, m_strTable));
            AfxMessageBox(strMessage);
            m_oCommand.Close();
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        hResult = m_oCommand.MoveNext();
        if (hResult == DB_S_ENDOFROWSET)
        {
            AfxMessageBox(_T("No %s found with the specified ID to update.", m_strTable));
            m_oCommand.Close();
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        TRecord& oDatabaseRecord = m_oCommand.GetRecord();
        if (oRecord.lUpdateCounter != oDatabaseRecord.lUpdateCounter)
        {
            AfxMessageBox(_T("Update counters do not match in the database"));
            m_oCommand.Close();
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }

        oDatabaseRecord = oRecord;
        oDatabaseRecord.lUpdateCounter += 1;
        oRecord.lUpdateCounter += 1;

        hResult = m_oCommand.SetData(DATA_ACCESSOR_INDEX);
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
            m_oCommand.Close();
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
            m_oCommand.Close();
            m_oSessionManager.CloseSession();
            return false;
        }
        m_oCommand.Close();
        return true;
    }
    /// <summary>Вмъква нов потребител в базата</summary>  
    bool Insert(TRecord& oRecord)
    {
        CString strQuery;
        strQuery.Format(SQL_SELECT_EMPTY, m_strTable);
        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        if (!m_oSessionManager.ExecuteQuery(strQuery, m_oCommand, &oUpdateDBPropSet))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to update %s oRecord. Error: %d", hResult, m_strTable));
            AfxMessageBox(strMessage);
            m_oCommand.Close();
            m_oSessionManager.CloseSession();
            return false;
        }

        TRecord& oDatabaseRecord = m_oCommand.GetRecord();
        oDatabaseRecord = oRecord;

        HRESULT hResult = m_oCommand.Insert(DATA_ACCESSOR_INDEX);
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
            m_oCommand.Close();
            m_oSessionManager.CloseSession();

            return false;
        }

        hResult = m_oCommand.MoveFirst();
        if (hResult != S_OK)
        {
            AfxMessageBox(_T("Unable to read inserted %s oRecord again from the SQL Server database. Error: %d", m_strTable, hResult));
            m_oCommand.Close();
            m_oSessionManager.CloseSession();
            return false;
        }
        oRecord.lId = oDatabaseRecord.lId;

        m_oCommand.Close();
        return true;
    }
    /// <summary>Изтрива потребител от базата според ID</summary>  
    bool DeleteWhereID(const long lID)
    {
        CString strQuery;
        strQuery.Format(SQL_SELECT_BY_ID, m_strTable, lID);
        CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

        HRESULT hResult = m_oSessionManager.BeginTransaction();
        if (FAILED(hResult))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to begin transaction. Error: %d", hResult));
            AfxMessageBox(strMessage);
            m_oSessionManager.CloseSession();
            return false;
        }

        if (!m_oSessionManager.ExecuteQuery(strQuery, m_oCommand, &oUpdateDBPropSet))
        {
            CString strMessage;
            strMessage.Format(_T("Failed to delete %s oRecord. Error: %d", m_strTable, hResult));
            AfxMessageBox(strMessage);
            m_oCommand.Close();
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();

            return false;
        }

        hResult = m_oCommand.MoveFirst();
        if (hResult != S_OK)
        {
            AfxMessageBox(_T("No %s oRecord found with the specified ID", m_strTable));
            m_oCommand.Close();
            m_oSessionManager.RollbackTransaction();
            m_oSessionManager.CloseSession();
            return false;
        }
        hResult = m_oCommand.Delete();
        if (FAILED(hResult))
        {
            AfxMessageBox(_T("Failed to delete the %s. oRecord", m_strTable));
            m_oCommand.Close();
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
            m_oCommand.Close();
            m_oSessionManager.CloseSession();
            return false;
        }
        m_oCommand.Close();
        return true;
    }

    // Members
    // ----------------
private:
    /// <summary>Мениджър на сесията</summary>  
    CSessionManager& m_oSessionManager; 
    /// <summary>Команда</summary>  
    CCommand<CAccessor<TAccessor>> m_oCommand;
    /// <summary>От коя таблица ще се извличат данни в заявките</summary>
    CString m_strTable;
};