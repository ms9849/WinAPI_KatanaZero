#include "pch.h"
#include "CFollowEffect.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CFollowEffect::CFollowEffect()
{
}

CFollowEffect::~CFollowEffect()
{
}

void CFollowEffect::Initialize()
{
	if (m_pFrameKey == L"Right_Effect_Blood")
	{
		m_pFrameKey = L"Effect_Blood";
		m_tInfo.fCX = 80.f;
		m_tInfo.fCY = 79.f;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 9;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 75.f;
	}

	if (m_pFrameKey == L"Left_Effect_Blood")
	{
		m_pFrameKey = L"Effect_Blood";
		m_tInfo.fCX = 80.f;
		m_tInfo.fCY = 79.f;
		m_tFrame.iStart = 9;
		m_tFrame.iEnd = 9;
		m_tFrame.iMotion = 1;
		m_tFrame.dwFrameSpeed = 75.f;
	}

	if (m_pFrameKey == L"Butcher_Knife_Effect")
	{
		m_pFrameKey = L"Butcher_Knife_Effect";
		m_tInfo.fCX = 42;
		m_tInfo.fCY = 42;
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.dwFrameSpeed = 10000.f;
	}
}

int CFollowEffect::Update()
{
    return NOEVENT;
}

void CFollowEffect::Late_Update()
{
}

void CFollowEffect::Render(HDC hDC)
{	//일단은 따라가게.. 문제 생기면 수정
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	m_tInfo.fX = m_pTarget->Get_SpriteInfo()->fX;
	m_tInfo.fY = m_pTarget->Get_SpriteInfo()->fY;

	if (m_pFrameKey != L"Butcher_Knife_Effect")
	{
		GdiTransparentBlt(
			hDC,
			m_tFrame.iMotion == 1 ? m_tInfo.fX - m_tInfo.fCX + iScrollX : m_tInfo.fX + iScrollX,
			m_tInfo.fY - m_tInfo.fCY + iScrollY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			RGB(255, 0, 255)
		);

		if (isAnimationEnd())
			m_bDead = true;
	}
	else if (m_pFrameKey == L"Butcher_Knife_Effect")
	{
		GdiTransparentBlt(
			hDC,
			m_tInfo.fX - m_tInfo.fCX / 2.f + iScrollX,
			m_tInfo.fY - m_tInfo.fCY / 2.f - 60 + iScrollY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			RGB(255, 0, 255)
		);

		if (GetTickCount64() - m_fEffectTime >= 600)
			m_bDead = true;
	}


	__super::Update_Frame();
}

void CFollowEffect::Release()
{
}
