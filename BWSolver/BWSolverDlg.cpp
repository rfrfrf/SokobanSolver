// BWSolverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BoxWorldSolver.h"
#include "BWSolverDlg.h"
#include "IconBrush.h"

#include "BWSolver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CBWSolverApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBWSolverDlg dialog

CBWSolverDlg::CBWSolverDlg(CWnd* pParent /*=NULL*/)
: CDialog(CBWSolverDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CBWSolverDlg)
    m_MaxSolutionDepth = 0;
    m_Priority = FALSE;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_ScrollBarFrame = 0;
}

void CBWSolverDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBWSolverDlg)
    DDX_Control(pDX, IDC_BUTTONSOLVE, m_ButtonSolveCtrl);
    DDX_Control(pDX, IDC_SOLUTIONPROGRESS, m_SolutionProgressCtrl);
    DDX_Control(pDX, IDC_MAXDEPTH, m_MaxSolutionDepthCtrl);
    DDX_Control(pDX, IDC_MAXDEPTHSPIN, m_MaxSolutionDepthSpinCtrl);
    DDX_Control(pDX, IDC_FRAMENUMBER, m_FrameNumberCtrl);
    DDX_Control(pDX, IDC_FRAMESLIDER, m_ScrollBarCtrl);
    DDX_Control(pDX, IDC_PALETTE, m_PaletteCtrl);
    DDX_Control(pDX, IDC_WORLD, m_WorldCtrl);
    DDX_Slider(pDX, IDC_FRAMESLIDER, m_ScrollBarFrame);
    DDX_Text(pDX, IDC_MAXDEPTH, m_MaxSolutionDepth);
    DDX_Control(pDX, IDC_INFORMATION, m_InformationTextCtrl);
    DDX_Check(pDX, IDC_PRIORITY, m_Priority);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBWSolverDlg, CDialog)
    //{{AFX_MSG_MAP(CBWSolverDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTONSOLVE, OnButtonSolve)
    ON_WM_HSCROLL()
    ON_NOTIFY(UDN_DELTAPOS, IDC_MAXDEPTHSPIN, OnDeltaposMaxDepthSpin)
    ON_BN_CLICKED(IDC_PRIORITY, OnPriority)
    ON_BN_CLICKED(IDC_LOADWORLD, OnLoadworld)
    ON_BN_CLICKED(IDCANCEL, OnQuit)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CBWSolverDlg::~CBWSolverDlg()
{
    //m_WorldCtrl.SaveToFile(m_WorldMapFilename);
}

/////////////////////////////////////////////////////////////////////////////
// CBWSolverDlg message handlers

BOOL CBWSolverDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    CIconBrush * brush = new CIconBrush();
    m_PaletteCtrl.SetSharedIconBrush(brush);
    m_WorldCtrl.SetSharedIconBrush(brush);

    m_WorldMapFilename = "maps\\default.txt";
    LoadWorldFromFile(m_WorldMapFilename);

    m_ScrollBarFrame = 1; 

    m_ScrollBarCtrl.SetRange( 1 , 1, FALSE); 
    m_ScrollBarCtrl.SetTicFreq(1); 
    m_ScrollBarCtrl.SetPos(m_ScrollBarFrame); 


    m_MaxSolutionDepth = 30;
    m_Solver.SetSolutionMaxDepth(m_MaxSolutionDepth);
    m_Solver.AddObserver(this);

    m_SolutionProgressCtrl.SetRange(0,100);
    m_SolutionProgressCtrl.SetStep(1);


    //m_ScrollBarCtrl.SetRangeMin(0);
    //m_ScrollBarCtrl.SetRangeMax(10, TRUE);

    m_SolutionPosition = 0;

    m_BusySolving = false;

    SetInformation("Box World Solver, Duisendpoot, South Africa \12duisendpoot@gmail.com");

    UpdateData(false);

    //SetScrollRange(1, m_Solver.GetSolutionDepth()-1, FALSE);


    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBWSolverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBWSolverDlg::UpdateControls()
{
    if (m_MaxSolutionDepth<3) m_MaxSolutionDepth = 3;
    if (m_MaxSolutionDepth>200) m_MaxSolutionDepth = 200;

    m_Solver.SetSolutionMaxDepth(m_MaxSolutionDepth);
    UpdateData(false);
}

void CBWSolverDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBWSolverDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}



void CBWSolverDlg::OnButtonSolve() 
{
    if (m_BusySolving)
    {
        m_Solver.StopSolving();
        return;
    }

    m_ButtonSolveCtrl.SetWindowText("Stop");
    m_BusySolving = true;

    SetInformation("");
    // TODO: Add your control notification handler code here
    UpdateControls();

    SetInformation("Clearing previous states...(this may take a few seconds)");
    m_Solver.Clear();
    m_Solver.SetWorldMap(*m_WorldCtrl.GetWorld());

    try
    {
        if (!m_Solver.Solve())
        {
            //MessageBox("Unable to solve map!", "Box World Solver", MB_ICONINFORMATION | MB_OK);
            SetInformation("Unable to solve map!");
        }
        else
        {
            //MessageBox("Solved!", "Box World Solver", MB_ICONINFORMATION | MB_OK);
        }
    } 
    catch (BWSolverException ex)
    {
        //MessageBox(ex.GetErrorMessage().c_str(), "Box World Solver", MB_ICONINFORMATION | MB_OK);
        SetInformation(ex.GetErrorMessage().c_str());
    }

    //m_FrameNumberCtrl.SetWindowText("Frame 1");
    //m_SolutionPosition = 1;

    m_ScrollBarCtrl.SetRange(1,m_Solver.GetSolutionDepth(),TRUE);
    UpdateData(true);

    m_ButtonSolveCtrl.SetWindowText("Solve");
    m_BusySolving = false;
}

void CBWSolverDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (m_BusySolving)
    {
        return; // need to stop solver first
    }

    UpdateData(true);

    // TODO: Add your message handler code here and/or call default
    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);




    if (m_Solver.GetSolutionDepth()<=0) return;


    if (nPos>0)
    {
        SetToFrame(nPos);
        /*
        m_SolutionPosition = nPos;

        std::ostringstream stream;
        stream << "Frame "<<nPos;
        m_FrameNumberCtrl.SetWindowText(stream.str().c_str());


        m_WorldCtrl.SetWorld(*m_Solver.GetSolutionWorldMap(m_SolutionPosition));
        m_WorldCtrl.Invalidate(FALSE);*/
    }
}

void CBWSolverDlg::SetToFrame(int nPos)
{
    m_SolutionPosition = nPos;
    std::ostringstream stream;
    stream << "Frame "<<nPos;
    m_FrameNumberCtrl.SetWindowText(stream.str().c_str());


    m_WorldCtrl.SetWorld(*m_Solver.GetSolutionWorldMap(m_SolutionPosition));
    m_WorldCtrl.Invalidate(FALSE);
}

void CBWSolverDlg::OnDeltaposMaxDepthSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    // TODO: Add your control notification handler code here

    int sol = m_MaxSolutionDepth;
    sol -= pNMUpDown->iDelta;
    if (sol<0) sol = 0;
    m_MaxSolutionDepth = sol;

    UpdateControls();

    *pResult = 0;
}




void CBWSolverDlg::OnOK() 
{
    /*char buff[100];

    m_MaxSolutionDepthCtrl.GetWindowText(buff, 100);
    SetFocus();
    m_MaxSolutionDepth = atoi(buff);
    UpdateControls();*/
    // TODO: Add extra validation here

    //	CDialog::OnOK();
}

void CBWSolverDlg::AddInformation(CString text)
{
    CString text2;
    m_InformationTextCtrl.GetWindowText(text2);
    m_InformationTextCtrl.SetWindowText(text2+text);
    UpdateData(true); 
}


void CBWSolverDlg::SetInformation(CString text)
{
    m_InformationTextCtrl.SetWindowText(text);
    UpdateData(false); 
}


void CBWSolverDlg::Progress(long depth,
                            long statesTraversed,
                            long statesStored,
                            long recurrentStates
                            )
{
    std::ostringstream stream;
    stream << statesTraversed<<" states traversed, "
        << statesStored<<" states stored using "
        << (statesStored*285)/1024<<"K and "
        << recurrentStates<<" recurrent states.";
    SetInformation(stream.str().c_str());
    m_SolutionProgressCtrl.SetPos(depth*100/m_MaxSolutionDepth);

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
        if (!AfxGetThread()->PumpMessage()) {}
    }
}

void CBWSolverDlg::Finished(int solutionDepth)
{
    m_SolutionProgressCtrl.SetPos(0);

    std::ostringstream stream;
    stream << "Solved in "<<solutionDepth<<" frames, with max depth "<<m_MaxSolutionDepth;

    AddInformation(stream.str().c_str());
}

void CBWSolverDlg::OnPriority() 
{
    // TODO: Add your control notification handler code here
    UpdateData();
}

void CBWSolverDlg::OnLoadworld() 
{
    if (m_BusySolving)
    {
        return; // need to stop solver first
    }
    // TODO: Add your control notification handler code here
    HRESULT hResult; 
    CString strFilter = "All Files (*.*)|*.*||";
    strFilter = "Box World Files (*.txt)|*.txt|" + strFilter;

    CFileDialog dlg(TRUE, NULL, m_WorldMapFilename.c_str(), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter); 
    hResult = (int)dlg.DoModal(); 
    if(hResult != IDOK) { 
        return; 
    } 

    m_WorldMapFilename = dlg.GetPathName();
    LoadWorldFromFile(m_WorldMapFilename); 
}

void CBWSolverDlg::LoadWorldFromFile(std::string WorldMapFilename) 
{
    SetInformation("Clearing previous states...(this may take a few seconds)");
    m_Solver.Clear();
    SetWindowText(std::string(("Box World Solver - ")+WorldMapFilename).c_str());
    m_WorldMapFilename = WorldMapFilename;
    m_WorldCtrl.LoadFromFile(m_WorldMapFilename);
    UpdateData(FALSE);
}


void CBWSolverDlg::OnQuit() 
{
    // TODO: Add your control notification handler code here
    if (m_BusySolving)
    {
        m_Solver.StopSolving();
        return;
    }

    int ret = IDYES;
    if (m_WorldCtrl.GetChangedFlag())
    {
        ret = MessageBox("You have unsaved changes. Really Quit?", "Box World Solver", MB_YESNO);
    }
    if (ret==IDYES) CDialog::OnCancel();
}



