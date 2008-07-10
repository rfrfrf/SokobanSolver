// BWSolver.h : main header file for the BWSOLVER application
//

#if !defined(AFX_BWSOLVER_H__36D98747_93F0_11D9_9525_0050BF096855__INCLUDED_)
#define AFX_BWSOLVER_H__36D98747_93F0_11D9_9525_0050BF096855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBWSolverApp:
// See BWSolver.cpp for the implementation of this class
//

class CBWSolverApp : public CWinApp
{
public:
	CBWSolverApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBWSolverApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBWSolverApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  public:
    HICON GetIcon(int n);
  private:
    void LoadIcons();
    HICON icons[10];
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BWSOLVER_H__36D98747_93F0_11D9_9525_0050BF096855__INCLUDED_)
