#include "pch.h"
#include "CLaser.h"
#include "CScrollMgr.h"

CLaser::CLaser()
{
}

CLaser::~CLaser()
{
	Release();
}


void CLaser::Initialize()
{
}

int CLaser::Update()
{
	return 0;
}

void CLaser::Late_Update()
{
}

void CLaser::Render(HDC hDC)
{
    int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

    if ((GetTickCount64() - m_fLaserTime > m_fLaserLife / g_fSlowFactor) || m_bDead == true)
    {
        m_bDead = true;
        return;
    }

    MoveToEx(hDC, m_tLeft.fX + iScrollX, m_tLeft.fY + iScrollY, NULL);
    LineTo(hDC, m_tRight.fX + iScrollX, m_tRight.fY + iScrollY);

    //(500 - GetTickCount64() - m_fLaserTime) * 0.04
    HPEN hPen = CreatePen(PS_SOLID, 25, RGB(255, 165, 0)); // »¡°£»ö ¿¹½Ã
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

    MoveToEx(hDC, m_tLeft.fX + iScrollX ,  m_tLeft.fY + iScrollY, NULL);
    LineTo(hDC, m_tRight.fX + iScrollX, m_tRight.fY + iScrollY);

    SelectObject(hDC, hOldPen);
    DeleteObject(hPen);
}

void CLaser::Release()
{
}
