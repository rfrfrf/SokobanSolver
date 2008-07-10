// DrawableMapCtrl.cpp: implementation of the CDrawableMapCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BWSolver.h"
#include "DrawableMapCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawableMapCtrl::CDrawableMapCtrl()
{
    world.SetXY(10,10);
    world.Clear();
    world.DrawBoundaries();
    m_DrawIcon = IconName::Empty;
}

CDrawableMapCtrl::~CDrawableMapCtrl()
{
}

// PaletteCtrl.cpp : implementation file
//


BEGIN_MESSAGE_MAP(CDrawableMapCtrl, CWorldMapCtrl)
    //{{AFX_MSG_MAP(CDrawableMapCtrl)
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDrawableMapCtrl::SetDrawIcon(IconName::Enum icon)
{
    m_DrawIcon = icon;
}


/////////////////////////////////////////////////////////////////////////////
// CPaletteCtrl message handlers

void CDrawableMapCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default

    IconName::Enum DrawNowIcon = IconName::Empty;

    CWorldMapCtrl::OnRButtonDown(nFlags, point);

    int m_SelectedIcon_x = point.x/32;
    int m_SelectedIcon_y = point.y/32;

    DrawEvent_BusinessLogic(m_SelectedIcon_x,m_SelectedIcon_y, DrawNowIcon);
    SetWorldIcon(m_SelectedIcon_x,m_SelectedIcon_y, DrawNowIcon);

    Invalidate(FALSE);
}

void CDrawableMapCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default

    IconName::Enum DrawNowIcon = m_DrawIcon;

    if (m_SharedIconBrush!=NULL) 
    {
        DrawNowIcon = m_SharedIconBrush->GetBrushIcon();
    }

    CWorldMapCtrl::OnLButtonDown(nFlags, point);
    int m_SelectedIcon_x = point.x/32;
    int m_SelectedIcon_y = point.y/32;

    DrawEvent_BusinessLogic(m_SelectedIcon_x,m_SelectedIcon_y, DrawNowIcon);
    SetWorldIcon(m_SelectedIcon_x,m_SelectedIcon_y, DrawNowIcon);


    Invalidate(FALSE);
}


void CDrawableMapCtrl::DrawEvent_BusinessLogic(int x,int y, IconName::Enum &iconToDraw)
{
    // Business logic
    if (iconToDraw == IconName::Player) 
    {
        ReplaceWorldIcons(IconName::Player, IconName::Empty);
        ReplaceWorldIcons(IconName::SpotPlayer, IconName::Spot);
    }

    if ((iconToDraw==IconName::Empty) &&
        (GetWorldIcon(x ,y) == IconName::RedBox))
    {
        iconToDraw = IconName::Spot;
    } else
        if ((iconToDraw==IconName::Box) &&
            (GetWorldIcon(x ,y) == IconName::Spot))
        {
            iconToDraw = IconName::RedBox;
        } else
            if ((iconToDraw==IconName::Empty) &&
                (GetWorldIcon(x ,y) == IconName::SpotPlayer))
            {
                iconToDraw = IconName::Spot;
            } else
                if ((iconToDraw==IconName::Player) &&
                    (GetWorldIcon(x ,y) == IconName::Spot))
                {
                    iconToDraw = IconName::SpotPlayer;
                }

}
