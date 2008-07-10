#if !defined(AFX_WORLDMAPCTRL_H__83CF2621_93F2_11D9_9525_0050BF096855__INCLUDED_)
#define AFX_WORLDMAPCTRL_H__83CF2621_93F2_11D9_9525_0050BF096855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorldMapCtrl.h : header file
//

#include "IconBrush.h"

/////////////////////////////////////////////////////////////////////////////
// CWorldMapCtrl window

class CWorldMapCtrl : public CWnd
{
    //DECLARE_DYNCREATE(CWorldMapCtrl)
    // Construction
public:
    CWorldMapCtrl();

    // Attributes
public:

    void SetSharedIconBrush(CIconBrush * brush);
    void LoadFromFile(std::string filename);
    void SaveToFile(std::string filename);
    WorldMap * GetWorld();
    void SetWorld(WorldMap & world);

    bool GetChangedFlag();
    void SetChangedFlag(bool f);

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWorldMapCtrl)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CWorldMapCtrl();

    // Generated message map functions
protected:
    //{{AFX_MSG(CWorldMapCtrl)
    afx_msg void OnPaint();
    //}}AFX_MSG

    virtual void Draw(CDC & dc);

    static LPCTSTR DetailWndClassName;

    IconName::Enum GetWorldIcon(int x, int y);
    void SetWorldIcon(int x, int y, IconName::Enum icon);

    BOOL RegisterWndClass();

    void TilesToIcon(Tile::Enum t1, Tile::Enum t2, IconName::Enum & icon);
    void IconToTiles(IconName::Enum  icon, Tile::Enum &t1, Tile::Enum &t2);
    void ReplaceWorldIcons(IconName::Enum source, IconName::Enum dest);

    DECLARE_MESSAGE_MAP()

    CIconBrush * m_SharedIconBrush;
    WorldMap world;
    bool m_ChangedFlag;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDMAPCTRL_H__83CF2621_93F2_11D9_9525_0050BF096855__INCLUDED_)
