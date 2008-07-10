// PaletteCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BWSolver.h"
#include "PaletteCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPaletteCtrl

CPaletteCtrl::CPaletteCtrl() :
m_SelectedIcon_x(0),
m_SelectedIcon_y(0)
{
    world.SetXY(3,3);
    //world.DrawBoundaries();
    SetWorldIcon(0,0,IconName::Empty); 
    SetWorldIcon(1,0,IconName::Wall); 
    SetWorldIcon(2,0,IconName::Spot); 
    SetWorldIcon(0,1,IconName::Box); 
    SetWorldIcon(1,1,IconName::RedBox); 
    SetWorldIcon(2,1,IconName::Player); 
}

CPaletteCtrl::~CPaletteCtrl()
{
}


BEGIN_MESSAGE_MAP(CPaletteCtrl, CWorldMapCtrl)
    //{{AFX_MSG_MAP(CPaletteCtrl)
    ON_WM_LBUTTONDOWN()
    //	ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteCtrl message handlers

void CPaletteCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default

    CWorldMapCtrl::OnLButtonDown(nFlags, point);

    m_SelectedIcon_x = point.x/32;
    m_SelectedIcon_y = point.y/32;

    if (m_SharedIconBrush!=NULL)
    {
        m_SharedIconBrush->SetBrushIcon(GetSelectedIcon());
    }

    Invalidate(FALSE);
}


void CPaletteCtrl::Draw(CDC  & dc)
{
    CWorldMapCtrl::Draw(dc);

    COLORREF Color1 = GetSysColor(COLOR_3DHILIGHT);
    COLORREF Color2 = GetSysColor(COLOR_3DSHADOW);

    for (int j=0;j<8;j++)
    {
        CRect rect(
            m_SelectedIcon_x*32+j*2,
            m_SelectedIcon_y*32+j*2,
            m_SelectedIcon_x*32+32-j*2,
            m_SelectedIcon_y*32+32-j*2
            );
        dc.Draw3dRect(&rect, Color2, Color1);
    }
}


IconName::Enum CPaletteCtrl::GetSelectedIcon()
{
    return GetWorldIcon(m_SelectedIcon_x, m_SelectedIcon_y);
}
