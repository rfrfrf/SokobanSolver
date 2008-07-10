// IconBrush.h: interface for the CIconBrush class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONBRUSH_H__86EF5FC5_BF84_455F_93C6_65F08940667E__INCLUDED_)
#define AFX_ICONBRUSH_H__86EF5FC5_BF84_455F_93C6_65F08940667E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIconBrush  
{
public:
    CIconBrush();
    virtual ~CIconBrush();

    IconName::Enum GetBrushIcon();
    void SetBrushIcon(IconName::Enum icon);
private:
    IconName::Enum m_BrushIcon;
};

#endif // !defined(AFX_ICONBRUSH_H__86EF5FC5_BF84_455F_93C6_65F08940667E__INCLUDED_)
