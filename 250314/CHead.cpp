#include "pch.h"
#include "CHead.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CHead::CHead()
{
}

CHead::~CHead()
{
    Release();
}

void CHead::Initialize()
{
    m_fHeadBanging = 0.f;
    m_bJump = true;
    m_fTime = 0;
    //좌표는 추상 팩토리에서 넣어줄 것
    m_fSpeed = 3.f;
    m_tInfo.fCX = 37.f;
    m_tInfo.fCY = 25.f;
    m_pFrameKey = L"Headhunter_Head";
    m_tFrame.iStart = 0;
    m_tFrame.iEnd = 0;
    m_tFrame.iMotion = 0;
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_head.bmp", L"Headhunter_Head");
}

int CHead::Update()
{
    return NOEVENT;
}

void CHead::Late_Update()
{
    //여기에 공격 각도에 따라 로직 추가할 것.

    if (m_bJump)
    {
        m_tInfo.fX += 3.0 * m_fSpeed * cosf(m_fAngle * PI / 180.f) * g_fSlowFactor;

        if (12.f * m_fTime - 0.5f * 4.9f * m_fTime * m_fTime >= -10.0f)
            m_tInfo.fY -= (12.f * m_fTime - 0.5f * 4.9f * m_fTime * m_fTime) * g_fSlowFactor;
        else
            m_tInfo.fY -= -10.0f * g_fSlowFactor;

        m_fTime += 0.2f * g_fSlowFactor;
    }
}

void CHead::Render(HDC hDC)
{
	int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,/// 복사 받을 dc
		m_tInfo.fX - m_tInfo.fCX / 2.f + iScrollX,		// 복사 받을 위치 좌표 left
		m_tInfo.fY - m_tInfo.fCY / 2.f + iScrollY,					// 복사 받을 위치 좌표 top
		(int)m_tInfo.fCX,				// 복사 받을 가로 사이즈
		(int)m_tInfo.fCY,				// 복사 받을 세로 사이즈
		hMemDC,							// 복사할 이미지 dc
		m_tFrame.iStart * (int)m_tInfo.fCX,
		m_tFrame.iMotion * (int)m_tInfo.fCY,							// 복사할 이미지의 left, top
		(int)m_tInfo.fCX,				// 복사할 이미지의 가로
		(int)m_tInfo.fCY,				// 복사할 이미지의 세로
		RGB(255, 0, 255));
}

void CHead::Release()
{
}

void CHead::HitActor(CObj* HitActor, HITPOINT _HitInfo)
{
}

void CHead::HitActor(CMap* HitActor, HITPOINT _HitInfo)
{
    if (HitActor->GetObjType() == TYPE_GROUND) // 선 충돌.
    {
        m_fAngle = 0;
        m_fTime = 0.f;
        m_bJump = false;
    }

    if (HitActor->GetObjType() == TYPE_WALL || HitActor->GetObjType() == TYPE_MONSTER_WALL) // 렉트 벽 충돌
    {
        if (_HitInfo.eDirection == DIRECTION::DIR_DOWN)
        {
            m_tInfo.fY -= fabsf(_HitInfo.fY);
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_UP)
        {
            m_tInfo.fY += fabsf(_HitInfo.fY);
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_LEFT)
        {
            m_tInfo.fX += fabsf(_HitInfo.fX);
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_RIGHT)
        {
            m_tInfo.fX -= fabsf(_HitInfo.fX);
        }
    }
}

void CHead::HitActor(CObj* HitActor)
{
}
