#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ClientApp.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CSoftInternshipProjectDoc.h"
#include "CSoftInternshipProjectView.h"

#include "DLLExport.h"
#include "CUsersTable.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClientApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &ClientApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

ClientApp::ClientApp() noexcept
{
	SetAppID(_T("CSoftInternshipProject.AppID.NoVersion"));
}

ClientApp theApp;

BOOL ClientApp::InitInstance()
{
	CWinApp::InitInstance();
	//temp Domain/UserTable debuging:
	
	CUsersTable oUsers;


	/*CTypedArray<CPtrArray, USERS*> oUsers;

	USERS* pUser1 = new USERS(_T("Иван"), _T("ivan@example.com"), _T("Разработчик"));
	USERS* pUser2 = new USERS(_T("Петър"), _T("petar@example.com"), _T("Ръководител"));
	USERS* pUser3 = new USERS(_T("Георги"), _T("georgi@example.com"), _T("Разработчик"));

	oUsersPtrArray.Add(pUser1);
	oUsersPtrArray.Add(pUser2);
	oUsersPtrArray.Add(pUser3);

	CUsersTable.LoadUsers();*/

	

	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_CSoftInternshipProjectTYPE,
		RUNTIME_CLASS(CCSoftInternshipProjectDocument),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CCSoftInternshipProjectView));

	if (!pDocTemplate)
		return FALSE;

	AddDocTemplate(pDocTemplate);

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int ClientApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void ClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}