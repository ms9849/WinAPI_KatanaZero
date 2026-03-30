#include "pch.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CEffect.h"
#include "CRemainEffect.h"
#include "CEffectMgr.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
    m_bParried = false;
    m_bDead = false;
    m_fSpeed = 20.f;
    m_tInfo.fCX = 10.f;
    m_tInfo.fCY = 20.f;
}

int CBullet::Update()
{
    __super::Update_Rect();
	return NOEVENT;
}

void CBullet::Late_Update()
{
    if (fabsf(m_tInfo.fX) >= 6000 || fabsf(m_tInfo.fY) >= 6000)
        m_bDead = true;

    if (m_bDead) 
        return;

    m_tInfo.fX = m_tInfo.fX + g_fSlowFactor * m_fSpeed * cosf(m_fAngle * PI / 180);
    m_tInfo.fY = m_tInfo.fY - g_fSlowFactor * m_fSpeed * sinf(m_fAngle * PI / 180);
}

void CBullet::Render(HDC hDC)
{
    if (m_bDead) return;

    int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

    POINT   tLeft, tRight;

    float fLength = 60.0f;

    tLeft.x = m_tInfo.fX - fLength / 2 * cosf(m_fAngle * PI / 180.f);
    tLeft.y = m_tInfo.fY + fLength / 2 * sinf(m_fAngle * PI / 180.f); // +sinf → 아래로 내려감

    tRight.x = m_tInfo.fX + fLength / 2 * cosf(m_fAngle * PI / 180.f);
    tRight.y = m_tInfo.fY - fLength / 2 * sinf(m_fAngle * PI / 180.f); // -sinf → 위로 올라감

    if (tRight.y < tLeft.y)
    {
        POINT tmp = tRight;
        tRight = tLeft;
        tLeft = tmp;
    }

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0)); // 빨간색 예시
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

    MoveToEx(hDC, tLeft.x + iScrollX, tLeft.y + iScrollY, NULL);
    LineTo(hDC, tRight.x + iScrollX, tRight.y + iScrollY);

    //Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

    SelectObject(hDC, hOldPen);
    DeleteObject(hPen);
}

void CBullet::Release()
{
}

void CBullet::HitActor(CObj* HitActor, HITPOINT _HitInfo)
{
}

void CBullet::HitActor(CMap* HitActor, HITPOINT _HitInfo)
{
    //어디든 닿는다면
    if (HitActor->GetObjType() == TYPE_LASER) return;

    m_bDead = true;
}

void CBullet::HitActor(CObj* HitActor)
{
    if (m_bDead) return;

    if ((HitActor->Get_Type() == ITEM && dynamic_cast<CItem*>(HitActor)->Get_Throw() == false) || HitActor->Get_Dead() == true)
        return;

    //else if (HitActor->Get_Type() == ITEM && dynamic_cast<CItem*>(HitActor)->Get_Throw() == true && m_bParried)
      //  return;

    if (HitActor->Get_Type() == PLAYER)
    {
        if (m_bParried) return;
        else m_bDead = true;

        return;
    }

    else if (HitActor->Get_Type() == MONSTER)
    {
        if (!m_bParried || HitActor->Get_Dead() == true) 
            return;
        else 
            m_bDead = true;

        return;
    }
    //패링 시 몬스터 타격하게

    if (!m_bParried)
    {
        CEffect* pEffect = nullptr;
        pEffect = new CRemainEffect(m_tInfo.fX, m_tInfo.fY, L"Bullet_Reflect");
        pEffect->Initialize();
        CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);
    }
    
    Reverse();
    m_bParried = true;

    if(g_bSlowMod)
        CScrollMgr::Get_Instance()->Shake(15, 200); //ms단위로 시간 입력.
    else
        CScrollMgr::Get_Instance()->Shake(10, 200);
}
