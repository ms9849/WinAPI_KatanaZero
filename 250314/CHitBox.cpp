#include "pch.h"
#include "CHitBox.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CEffectMgr.h"

CHitBox::CHitBox() : m_pTarget(nullptr), m_fDist(30.f), m_fTargetAngle(0.f), m_bHit(false)
{
}

CHitBox::~CHitBox()
{
	Release();
}

void CHitBox::Initialize()
{
	if (m_eCreator == PLAYER)
	{
		m_tSpriteInfo.fCX = 178.f;
		m_tSpriteInfo.fCY = 178.f;
		m_tInfo.fCX = 80.f;
		m_tInfo.fCY = 80.f;
		m_fDist = 20.f;

		m_pFrameKey = L"Player_Hit";
		m_tFrame.dwFrameSpeed = 35.f; //추후 time 변수 추가

		if (m_fTargetAngle < 90 || m_fTargetAngle > 270)
		{
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 7;
			m_tFrame.iMotion = 0;
		}
		else
		{
			m_tFrame.iStart = 7;
			m_tFrame.iEnd = 7;
			m_tFrame.iMotion = 1;
		}
	}
	else if (m_eCreator == GRUNT)
	{
		m_tSpriteInfo.fCX = 120.f;
		m_tSpriteInfo.fCY = 74.f;
		m_tInfo.fCX = 50.f;
		m_tInfo.fCY = 50.f;
		m_fDist = 20.f;

		m_pFrameKey = L"Grunt_Hit";
		m_tFrame.dwFrameSpeed = 50.f; //추후 time 변수 추가

		if (m_fTargetAngle < 90 || m_fTargetAngle > 270)
		{
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 4;
			m_tFrame.iMotion = 0;
		}
		else
		{
			m_tFrame.iStart = 4;
			m_tFrame.iEnd = 4;
			m_tFrame.iMotion = 1;
		}
	}
	//Pomp, HeadHunter 추가 해야한다.
	m_tFrame.dwTime = GetTickCount64();
}

int CHitBox::Update()
{
	if ((m_tFrame.iMotion == 0 && m_tFrame.iStart == m_tFrame.iEnd) ||
		(m_tFrame.iMotion == 1 && m_tFrame.iStart == 0))
		return DEAD;

	return NOEVENT;
}

void CHitBox::Late_Update()
{
	m_tInfo.fX = m_pTarget->Get_Info()->fX + cosf(m_fTargetAngle * PI / 180) * m_fDist;
	m_tInfo.fY = m_pTarget->Get_Info()->fY - sinf(m_fTargetAngle * PI / 180) * m_fDist;
	__super::Update_Rect();

	float	fDistance = sqrtf((m_tSpriteInfo.fCX / 2.f) * (m_tSpriteInfo.fCX / 2.f) + (m_tSpriteInfo.fCY / 2.f) * (m_tSpriteInfo.fCY / 2.f));

	if (m_fTargetAngle < 90 || m_fTargetAngle > 270)
	{
		m_tPoint[0].x = LONG((m_tSpriteInfo.fCX / 2.f) + fDistance * cosf((m_fTargetAngle + 135.f) * PI / 180.f));
		m_tPoint[0].y = LONG((m_tSpriteInfo.fCY / 2.f) - fDistance * sinf((m_fTargetAngle + 135.f) * PI / 180.f));

		// 우 상단

		m_tPoint[1].x = LONG((m_tSpriteInfo.fCX / 2.f) + fDistance * cosf((m_fTargetAngle + 45.f) * PI / 180.f));
		m_tPoint[1].y = LONG((m_tSpriteInfo.fCY / 2.f) - fDistance * sinf((m_fTargetAngle + 45.f) * PI / 180.f));

		// 좌 하단

		m_tPoint[2].x = LONG((m_tSpriteInfo.fCX / 2.f) + fDistance * cosf((m_fTargetAngle + 225.f) * PI / 180.f));
		m_tPoint[2].y = LONG((m_tSpriteInfo.fCY / 2.f) - fDistance * sinf((m_fTargetAngle + 225.f) * PI / 180.f));
	}

	else
	{
		m_tPoint[0].x = LONG((m_tSpriteInfo.fCX / 2.f) + fDistance * cosf((m_fTargetAngle + 135.f + 180.f) * PI / 180.f));
		m_tPoint[0].y = LONG((m_tSpriteInfo.fCY / 2.f) - fDistance * sinf((m_fTargetAngle + 135.f + 180.f) * PI / 180.f));

		// 우 상단

		m_tPoint[1].x = LONG((m_tSpriteInfo.fCX / 2.f) + fDistance * cosf((m_fTargetAngle + 45.f + 180.f) * PI / 180.f));
		m_tPoint[1].y = LONG((m_tSpriteInfo.fCY / 2.f) - fDistance * sinf((m_fTargetAngle + 45.f + 180.f) * PI / 180.f));

		// 좌 하단

		m_tPoint[2].x = LONG((m_tSpriteInfo.fCX / 2.f) + fDistance * cosf((m_fTargetAngle + 225.f + 180.f) * PI / 180.f));
		m_tPoint[2].y = LONG((m_tSpriteInfo.fCY / 2.f) - fDistance * sinf((m_fTargetAngle + 225.f + 180.f) * PI / 180.f));
	}

	__super::Update_Frame();
	__super::Update_Sprite();
}

void CHitBox::Render(HDC hDC)
{
	int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_eCreator == PLAYER)
	{

		HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		HDC		hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"Plg");
		HDC		hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"Reset");

		BitBlt(hResetDC, 0, 0, m_tSpriteInfo.fCX, m_tSpriteInfo.fCY, hMemDC,
			m_tFrame.iStart * m_tSpriteInfo.fCX,
			m_tFrame.iMotion * m_tSpriteInfo.fCY,
			SRCCOPY);

		PlgBlt(hPlgDC, m_tPoint, hResetDC, 0, 0, m_tSpriteInfo.fCX, m_tSpriteInfo.fCY, NULL, NULL, NULL);

		GdiTransparentBlt(hDC,
			m_tSpriteInfo.fX - m_tSpriteInfo.fCX / 2.f + iScrollX,
			m_tSpriteInfo.fY - m_tSpriteInfo.fCY / 2.f + iScrollY,
			m_tSpriteInfo.fCX, m_tSpriteInfo.fCY,
			hPlgDC, 0, 0, m_tSpriteInfo.fCX, m_tSpriteInfo.fCY, RGB(255, 0, 255));

	}
	//hitbox size
	//Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
}

void CHitBox::Release()
{
}
