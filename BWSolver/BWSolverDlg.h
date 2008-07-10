// BWSolverDlg.h : header file
//

#if !defined(AFX_BWSOLVERDLG_H__36D98749_93F0_11D9_9525_0050BF096855__INCLUDED_)
#define AFX_BWSOLVERDLG_H__36D98749_93F0_11D9_9525_0050BF096855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DrawableMapCtrl.h"
#include "PaletteCtrl.h"
#include "BoxWorldSolver.h"
#include "BWSolver.h"
#include "BWSolverObserver.h"

/////////////////////////////////////////////////////////////////////////////
// CBWSolverDlg dialog

class CBWSolverDlg : public CDialog, public BWSolverObserver
{
    // Construction
public:
    CBWSolverDlg(CWnd* pParent = NULL);	// standard constructor

    ~CBWSolverDlg();

    // Dialog Data
    //{{AFX_DATA(CBWSolverDlg)
    enum { IDD = IDD_BWSOLVER_DIALOG };
    CButton	m_ButtonSolveCtrl;
    CProgressCtrl	m_SolutionProgressCtrl;
    CEdit	m_MaxSolutionDepthCtrl;
    CSpinButtonCtrl	m_MaxSolutionDepthSpinCtrl;
    CStatic	m_FrameNumberCtrl;
    CSliderCtrl	m_ScrollBarCtrl;
    CPaletteCtrl	    m_PaletteCtrl;
    CDrawableMapCtrl	m_WorldCtrl;
    int               m_ScrollBarFrame;
    UINT	m_MaxSolutionDepth;
    CStatic	m_InformationTextCtrl;
    BOOL	m_Priority;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBWSolverDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

    void LoadWorldFromFile(std::string WorldMapFilename);
    void SetInformation(CString text);
    void AddInformation(CString text);
    void SetToFrame(int nPos);

    virtual void Progress(long depth,
        long statesTraversed,
        long statesStored,
        long recurrentStoredStates
        );
    virtual void Finished(int solutionDepth);

    // Implementation
protected:
    HICON m_hIcon;

    BWSolver m_Solver;
    int m_SolutionPosition;
    std::string m_WorldMapFilename;

    bool m_BusySolving;

    void UpdateControls();

    // Generated message map functions
    //{{AFX_MSG(CBWSolverDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnButtonSolve();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnDeltaposMaxDepthSpin(NMHDR* pNMHDR, LRESULT* pResult);
    virtual void OnOK();
    afx_msg void OnPriority();
    afx_msg void OnLoadworld();
    afx_msg void OnQuit();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BWSOLVERDLG_H__36D98749_93F0_11D9_9525_0050BF096855__INCLUDED_)
