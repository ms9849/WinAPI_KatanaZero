#include "pch.h"
#include "CItem.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CEffectMgr.h"

CItem::CItem()
{
}

CItem::~CItem()
{
    Release();
}

void CItem::Initialize()
{
    m_fAngle = 0.f;
    m_bThrowing = false;
    m_bParried = false;
    m_fThrowingAngle = 0.f;
    m_tInfo.fCX = 34.f;
    m_tInfo.fCY = 34.f;
    m_fSpeed = 20.f;
    m_pImage = new Image(L"../Image/Item/Butcher_Knife.png");
}

int CItem::Update()
{
    if (fabsf(m_tInfo.fX) >= 5000 || fabsf(m_tInfo.fY) >= 5000)
        m_bDead = true;

    __super::Update_Rect();
    return NOEVENT;

}

void CItem::Late_Update()
{
    if (m_bDead) return;
    //m_tInfo.fX = m_tInfo.fX + g_fSlowFactor * m_fSpeed * cosf(m_fAngle * PI / 180);
    //m_tInfo.fY = m_tInfo.fY - g_fSlowFactor * m_fSpeed * sinf(m_fAngle * PI / 180);

    if (m_bThrowing == true)
    {
        m_fAngle -= 10.f;
        m_tInfo.fX = m_tInfo.fX + g_fSlowFactor * m_fSpeed * cosf(m_fThrowingAngle * PI / 180);
        m_tInfo.fY = m_tInfo.fY - g_fSlowFactor * m_fSpeed * sinf(m_fThrowingAngle * PI / 180);

    }
}

void CItem::Render(HDC hDC)
{
    if (m_bDead) return;

    int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Butcher_Knife");

    /*
    * 히트박스
        Rectangle(hDC, 
        m_tInfo.fX - m_tInfo.fCX / 2.f + iScrollX, m_tInfo.fY - m_tInfo.fCY / 2.f + iScrollY,
        m_tInfo.fX + m_tInfo.fCX / 2.f + iScrollX, m_tInfo.fY + m_tInfo.fCY / 2.f + iScrollY
        );
    */

    Graphics g(hDC);
    g.TranslateTransform((INT)(m_tInfo.fX + iScrollX), (INT)(m_tInfo.fY + iScrollY));
    g.RotateTransform(m_fAngle);
    g.TranslateTransform((INT)(-m_tInfo.fCX / 2.f), (INT)(-m_tInfo.fCY / 2.f));
    g.DrawImage(m_pImage, 0, 0, (INT)m_tInfo.fCX, (INT)m_tInfo.fCY);
    g.ResetTransform();
}

void CItem::Release()
{
}

void CItem::HitActor(CMap* HitActor, HITPOINT _HitInfo)
{
    if (HitActor->GetObjType() == TYPE_GROUND) // 선 충돌.
    {
        m_fAngle = 0;
    }

    if (HitActor->GetObjType() == TYPE_WALL || HitActor->GetObjType() == TYPE_MONSTER_WALL) // 렉트 벽 충돌
    {
        if (_HitInfo.eDirection == DIRECTION::DIR_DOWN)
        {
            m_tInfo.fY -= fabsf(_HitInfo.fY);
            m_fAngle = 0;
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_UP)
        {
            m_tInfo.fY += fabsf(_HitInfo.fY);
            m_fAngle = 180;
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_LEFT)
        {
            m_tInfo.fX += fabsf(_HitInfo.fX);
            m_fAngle = 90;
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_RIGHT)
        {
            m_tInfo.fX -= fabsf(_HitInfo.fX);
            m_fAngle = 270;
        }
    }

    if (m_bThrowing == true)
    {
        m_bThrowing = false; //보스맵 레이저는 고려 안해도 된다. 보스방엔 안넣을거니까..
        CSoundMgr::Get_Instance()->PlaySound(L"player_land.wav", SOUND_EFFECT, g_fVolume);
    }
}

void CItem::HitActor(CObj* HitActor)
{
    if (m_bDead) return;

    if (m_bThrowing == true && (HitActor->Get_Type() == MONSTER || HitActor->Get_Type() == BULLET)
       && HitActor->Get_Dead() == false)
    {
        if (HitActor->Get_Type() == BULLET && dynamic_cast<CBullet*>(HitActor)->Get_Parried() == false && HitActor->Get_Dead() == false)
            m_fThrowingAngle = 180 + m_fThrowingAngle;

        else if(HitActor->Get_Type() == MONSTER)
            m_bDead = true;
    }
}
