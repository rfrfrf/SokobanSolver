// IconBrush.cpp: implementation of the CIconBrush class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BWSolver.h"
#include "IconBrush.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIconBrush::CIconBrush()
{

}

CIconBrush::~CIconBrush()
{

}


IconName::Enum CIconBrush::GetBrushIcon()
{
    return m_BrushIcon;
}

void CIconBrush::SetBrushIcon(IconName::Enum icon)
{
    m_BrushIcon = icon;
}