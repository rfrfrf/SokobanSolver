// DrawableMapCtrl.h: interface for the CDrawableMapCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWABLEMAPCTRL_H__83CF2626_93F2_11D9_9525_0050BF096855__INCLUDED_)
#define AFX_DRAWABLEMAPCTRL_H__83CF2626_93F2_11D9_9525_0050BF096855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorldMapCtrl.h"


class CDrawableMapCtrl : public CWorldMapCtrl  
{
public:
    CDrawableMapCtrl();
    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDrawableMapCtrl)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CDrawableMapCtrl();

    void SetDrawIcon(IconName::Enum icon);

    void DrawEvent_BusinessLogic(int x,int y, IconName::Enum & iconToDraw);

    // Generated message map functions
protected:
    //{{AFX_MSG(CDrawableMapCtrl)
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG

    IconName::Enum m_DrawIcon;


    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWABLEMAPCTRL_H__83CF2626_93F2_11D9_9525_0050BF096855__INCLUDED_)




