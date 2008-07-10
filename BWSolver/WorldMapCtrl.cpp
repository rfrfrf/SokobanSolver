// WorldMapCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BoxWorldSolver.h"
#include "WorldMapCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CBWSolverApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWorldMapCtrl

//IMPLEMENT_DYNCREATE(CWorldMapCtrl, CMDIChildWnd)

CWorldMapCtrl::CWorldMapCtrl() : m_SharedIconBrush(NULL), m_ChangedFlag(false)
{
    RegisterWndClass();

    //m_Font.CreateStockObject(DEFAULT_GUI_FONT);
}

CWorldMapCtrl::~CWorldMapCtrl()
{
}

LPCTSTR CWorldMapCtrl::DetailWndClassName = _T("__DETAIL_WORLDMAP_WND_CLASS");

BOOL CWorldMapCtrl::RegisterWndClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, DetailWndClassName, &wndcls)))
    {
        // register a new class
        wndcls.style            = /*CS_DBLCLKS |*/ CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = DetailWndClassName;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

bool CWorldMapCtrl::GetChangedFlag()
{
    return m_ChangedFlag;
}

void CWorldMapCtrl::SetChangedFlag(bool f)
{
    m_ChangedFlag = f;
}


void CWorldMapCtrl::LoadFromFile(std::string filename)
{
    world.Clear();
    world.LoadFromFile(filename);
    m_ChangedFlag = false;
    Invalidate(FALSE);
}

void CWorldMapCtrl::SaveToFile(std::string filename)
{
    world.SaveToFile(filename);
    m_ChangedFlag = false;
}


BEGIN_MESSAGE_MAP(CWorldMapCtrl, CWnd)
    //{{AFX_MSG_MAP(CWorldMapCtrl)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldMapCtrl message handlers

void CWorldMapCtrl::OnPaint()
{
    CPaintDC ScreenDC(this); // device context for painting

    CRect rc;
    GetClientRect(rc);

    CDC dc2;
    dc2.CreateCompatibleDC(&ScreenDC);
    CBitmap MemBitmap;
    MemBitmap.CreateCompatibleBitmap(&ScreenDC, rc.Width(), rc.Height());

    CBitmap* pOldBitmap;
    pOldBitmap = dc2.SelectObject(&MemBitmap);


    Draw(dc2);

    ScreenDC.BitBlt(0, 0, rc.Width(), rc.Height(), &dc2, 0, 0, SRCCOPY);

    // Restore the default bitmap:

    dc2.SelectObject(pOldBitmap);
    dc2.DeleteDC();
    // Delete the Memory Bitmap
    MemBitmap.DeleteObject();

    // Do not call CDialog::OnPaint() for painting messages
}

void CWorldMapCtrl::Draw(CDC  & dc)
{
    CRect rect;
    CBrush brush;
    brush.CreateSysColorBrush(8);
    GetClientRect(rect);
    dc.FillRect(rect, &brush);
    brush.DeleteObject();

    for (int rx=0;rx<world.GetX();rx++)
    {
        for (int ry=0;ry<world.GetY();ry++)
        {
            dc.DrawIcon(rx*32,ry*32, theApp.GetIcon(GetWorldIcon(rx,ry)));
        }
    }

}

void CWorldMapCtrl::TilesToIcon(Tile::Enum t0, Tile::Enum t1, IconName::Enum &icon)
{
    icon = IconName::Empty;
    if (t0 == Tile::BoxTarget)
    {
        switch(t1)
        {
        case Tile::Empty: icon = IconName::Spot;break;
        case Tile::Wall: icon = IconName::Empty;break;
        case Tile::Box: icon = IconName::RedBox;break;
        case Tile::Player: icon = IconName::SpotPlayer;break;
        }
    }
    else
    {
        switch(t1)
        {
        case Tile::Empty: icon = IconName::Empty;break;
        case Tile::Wall: icon = IconName::Wall;break;
        case Tile::Box: icon = IconName::Box;break;
        case Tile::Player: icon = IconName::Player;break;
        }
    }
}

void CWorldMapCtrl::IconToTiles(IconName::Enum  icon, Tile::Enum &t0, Tile::Enum &t1)
{
    t0 = Tile::Empty; t1 = Tile::Empty;

    switch (icon)
    {
    case IconName::Empty:  t0 = Tile::Empty; t1 = Tile::Empty; break;
    case IconName::Wall:   t0 = Tile::Empty; t1 = Tile::Wall; break;
    case IconName::Spot:   t0 = Tile::BoxTarget; t1 = Tile::Empty; break;
    case IconName::Box:    t0 = Tile::Empty; t1 = Tile::Box; break;
    case IconName::RedBox: t0 = Tile::BoxTarget; t1 = Tile::Box; break;
    case IconName::Player: t0 = Tile::Empty; t1 = Tile::Player; break;
    case IconName::SpotPlayer: t0 = Tile::BoxTarget; t1 = Tile::Player; break;
    }
}


IconName::Enum CWorldMapCtrl::GetWorldIcon(int x, int y)
{
    IconName::Enum icon = IconName::Empty;

    //  Tile::Enum tx = world.GetTile(2, x, y);
    //  if (tx != Tile::Player) tx = world.GetTile(1, x, y);
    //  TilesToIcon(world.GetTile(0, x, y), tx, icon);

    TilesToIcon(world.GetTile(0, x, y), world.GetTile(1, x, y), icon);
    return icon;
}

void CWorldMapCtrl::SetWorldIcon(int x, int y, IconName::Enum icon)
{
    m_ChangedFlag = true;
    Tile::Enum t0,t1;
    IconToTiles(icon, t0, t1);
    world.SetTile(0, x, y, t0);
    world.SetTile(1, x, y, t1);
}


void CWorldMapCtrl::SetSharedIconBrush(CIconBrush * brush)
{
    m_SharedIconBrush = brush;
}

void CWorldMapCtrl::ReplaceWorldIcons(IconName::Enum source, IconName::Enum dest)
{
    for (int rx=0;rx<world.GetX();rx++)
    {
        for (int ry=0;ry<world.GetY();ry++)
        {
            if (GetWorldIcon(rx,ry)==source)
            {
                SetWorldIcon(rx,ry,dest);
            }
        }
    }
}

WorldMap * CWorldMapCtrl::GetWorld()
{
    return &world;
}

void CWorldMapCtrl::SetWorld(WorldMap & worldx)
{
    m_ChangedFlag = false;
    world.Copy(worldx);
}