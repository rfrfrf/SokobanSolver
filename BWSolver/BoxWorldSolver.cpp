// BWSolver.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BoxWorldSolver.h"
#include "BWSolverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBWSolverApp

BEGIN_MESSAGE_MAP(CBWSolverApp, CWinApp)
    //{{AFX_MSG_MAP(CBWSolverApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBWSolverApp construction

CBWSolverApp::CBWSolverApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBWSolverApp object

CBWSolverApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBWSolverApp initialization

BOOL CBWSolverApp::InitInstance()
{
    AfxEnableControlContainer();

    //this->OpenDocumentFile("maps\\default.txt");
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    /*CDocTemplate * pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CTest2Doc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CTest2View));
    AddDocTemplate(pDocTemplate);*/


    LoadIcons();

    CBWSolverDlg dlg;
    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}


void CBWSolverApp::LoadIcons()
{  
    icons[IconName::Empty] = LoadIcon(IDI_EMPTYICON);
    icons[IconName::Wall] = LoadIcon(IDI_WALLICON);
    icons[IconName::Box] = LoadIcon(IDI_BOXICON);
    icons[IconName::RedBox] = LoadIcon(IDI_REDBOXICON);
    icons[IconName::Spot] = LoadIcon(IDI_SPOTICON);
    icons[IconName::Player] = LoadIcon(IDI_MANICON);
    icons[IconName::SpotPlayer] = LoadIcon(IDI_SPOTMANICON);
}

HICON CBWSolverApp::GetIcon(int n)
{
    return icons[n];
}
