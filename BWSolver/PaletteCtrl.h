#if !defined(AFX_PALETTECTRL_H__83CF2624_93F2_11D9_9525_0050BF096855__INCLUDED_)
#define AFX_PALETTECTRL_H__83CF2624_93F2_11D9_9525_0050BF096855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaletteCtrl.h : header file
//
#include "WorldMapCtrl.h"
#include "IconBrush.h"

/////////////////////////////////////////////////////////////////////////////
// CPaletteCtrl window

class CPaletteCtrl : public CWorldMapCtrl
{
    // Construction
public:
    CPaletteCtrl();

    IconName::Enum GetSelectedIcon();


    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPaletteCtrl)
    //}}AFX_VIRTUAL


    // Implementation
public:
    virtual ~CPaletteCtrl();

    // Generated message map functions
protected:
    //{{AFX_MSG(CPaletteCtrl)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //	afx_msg void OnPaint();
    //}}AFX_MSG

    virtual void Draw(CDC  & dc);

    int m_SelectedIcon_x;
    int m_SelectedIcon_y;


    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTECTRL_H__83CF2624_93F2_11D9_9525_0050BF096855__INCLUDED_)
