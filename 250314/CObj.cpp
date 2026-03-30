#include "pch.h"
#include "CObj.h"
#include "CMap.h"

CObj::CObj() : m_fSpeed(0.f), m_bDead(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left   = long(m_tInfo.fX - m_tInfo.fCX / 2.f);
	m_tRect.top    = long(m_tInfo.fY - m_tInfo.fCY / 2.f);
	m_tRect.right  = long(m_tInfo.fX + m_tInfo.fCX / 2.f);
	m_tRect.bottom = long(m_tInfo.fY + m_tInfo.fCY / 2.f);
}

void CObj::Update_Sprite()
{
	m_tSpriteInfo.fX = m_tInfo.fX;
	m_tSpriteInfo.fY = m_tInfo.fY;
}

void CObj::Update_Frame()
{
	if (!g_bSlowMod)
	{
		if (m_tFrame.iMotion == 1)
		{
			if (m_tFrame.dwTime + m_tFrame.dwFrameSpeed < GetTickCount64())
			{
				m_tFrame.iStart--;
				m_tFrame.dwTime = GetTickCount64();

				if (m_tFrame.iStart < 0)
					m_tFrame.iStart = m_tFrame.iEnd;
			}
		}

		if (m_tFrame.iMotion == 0)
		{
			if (m_tFrame.dwTime + m_tFrame.dwFrameSpeed < GetTickCount64())
			{
				m_tFrame.iStart++;
				m_tFrame.dwTime = GetTickCount64();

				if (m_tFrame.iStart > m_tFrame.iEnd) m_tFrame.iStart = 0;
			}
		}
	}
	else
	{
		if (m_tFrame.iMotion == 1)
		{
			if (m_tFrame.dwTime + m_tFrame.dwFrameSpeed / g_fSlowFactor < GetTickCount64())
			{
				m_tFrame.iStart--;
				m_tFrame.dwTime = GetTickCount64();

				if (m_tFrame.iStart < 0)
					m_tFrame.iStart = m_tFrame.iEnd;
			}
		}

		if (m_tFrame.iMotion == 0)
		{
			if (m_tFrame.dwTime + m_tFrame.dwFrameSpeed / g_fSlowFactor < GetTickCount64())
			{
				m_tFrame.iStart++;
				m_tFrame.dwTime = GetTickCount64();

				if (m_tFrame.iStart > m_tFrame.iEnd) m_tFrame.iStart = 0;
			}
		}
	}
}
