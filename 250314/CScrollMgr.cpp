#include "pch.h"
#include "CScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr() : m_fScrollX(0.f), m_fScrollY(0.f), m_fMaxX(800.f), m_fMaxY(600.f),m_fMinX(0.f), m_fMinY(0.f), m_bShaking(false)
{
}

CScrollMgr::~CScrollMgr()
{
}


void CScrollMgr::Late_Update()
{
	//이거 걍 저어기 maingame에서 굴려도 되나? 렌더에 영향 주진 않을거같은데..

	//쉐이킹
	if (m_bShaking && (GetTickCount64() - m_fStartTime < m_fTime))
	{
		if (rand() % 2 == 0)
			m_fScrollX += (rand() % (100)) / 100.f * m_fIntensity;
		else
			m_fScrollX -= (rand() % (100)) / 100.f * m_fIntensity;

		if (rand() % 2 == 0)
			m_fScrollY += (rand() % (100)) / 100.f * m_fIntensity; //0 ~ 1.0
		else
			m_fScrollY -= (rand() % (100)) / 100.f * m_fIntensity; //0 ~ 1.0
	}
	//끝나면 설정 바꿔줌.
	else if (m_bShaking && (GetTickCount64() - m_fStartTime > m_fTime))
	{
		m_fScrollX = m_fBeforeScrollX;
		m_fScrollY = m_fBeforeScrollY;
		m_bShaking = false;
	}
}

void CScrollMgr::Shake(float _fIntensity, float _fTime)
{
	if (!m_bShaking)
	{
		m_bShaking = true;
		m_fIntensity = _fIntensity;
		m_fStartTime = GetTickCount64();
		m_fTime = _fTime;
		m_fBeforeScrollX = m_fScrollX;
		m_fBeforeScrollY = m_fScrollY;

	}
}

void CScrollMgr::Scroll_Lock()
{
	if (0.f < m_fScrollX)
		m_fScrollX = 0.f;

	if (0.f < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCX - m_fMaxX > m_fScrollX)
		m_fScrollX = WINCX - m_fMaxX;

	if (WINCY - m_fMaxY > m_fScrollY)
		m_fScrollY = WINCY - m_fMaxY;
}
