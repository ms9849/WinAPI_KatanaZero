#include "pch.h"
#include "CLaser.h"
#include "CScrollMgr.h"
#include "CMovingLaser.h"
#include "CHeadHunter_2.h"
#include "CHeadHunter.h"

CMovingLaser::CMovingLaser()
{
}

CMovingLaser::~CMovingLaser()
{
    Release();
}


void CMovingLaser::Initialize()
{
}

int CMovingLaser::Update()
{
    return 0;
}

void CMovingLaser::Late_Update()
{
}

void CMovingLaser::Render(HDC hDC)
{
    if ((dynamic_cast<CHeadHunter*>(m_pTarget) != nullptr) && (dynamic_cast<CHeadHunter*>(m_pTarget)->Get_Motion() != CHeadHunter::LEFT_SWEEP &&
        dynamic_cast<CHeadHunter*>(m_pTarget)->Get_Motion() != CHeadHunter::RIGHT_SWEEP) || m_bDead == true || g_bGameOver == true ||
        (GetTickCount64() - m_fStartTime >= m_fLaserTime / g_fSlowFactor))
    {
        m_bDead = true;
        return;
    }

    if (GetTickCount64() - m_fColorTime >= 150 / g_fSlowFactor)
    {
        m_iColor++;
        m_fColorTime = GetTickCount64();
    }

    int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

    if ((dynamic_cast<CHeadHunter*>(m_pTarget) != nullptr))
    {

    }

    if (m_eDir == LEFT) m_fAngle -= 2.8f * g_fSlowFactor;
    else m_fAngle += 2.8f * g_fSlowFactor;

    float     fTargetX = m_pTarget->Get_Info()->fX;
    float     fTargetY = m_pTarget->Get_Info()->fY;

    float     fAddX = cosf(m_fAngle * PI / 180);
    float     fAddY = sinf(m_fAngle * PI / 180);

    m_tLeft.fX = fTargetX + 60 * fAddX;
    m_tLeft.fY = fTargetY + 60 * fAddY;

    m_tRight.fX = fTargetX + 1500 * fAddX;
    m_tRight.fY = fTargetY + 1500 * fAddY;

    if (m_tLeft.fX > m_tRight.fX)
    {
        LINEPOINT lptmp = m_tRight;
        m_tRight = m_tLeft;
        m_tLeft = lptmp;
    }

    HPEN hPen;

    if(m_iColor % 2 == 0)
        hPen = CreatePen(PS_SOLID, 25, RGB(255, 165, 0)); // 주황
    else
        hPen = CreatePen(PS_SOLID, 25, RGB(255, 255, 255)); // 하양


    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

    MoveToEx(hDC, m_tLeft.fX + iScrollX, m_tLeft.fY + iScrollY, NULL);
    LineTo(hDC, m_tRight.fX + iScrollX, m_tRight.fY + iScrollY);

    SelectObject(hDC, hOldPen);
    DeleteObject(hPen);
}

void CMovingLaser::Release()
{
}
