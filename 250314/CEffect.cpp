#include "pch.h"
#include "CEffect.h"

void CEffect::Update_Frame()
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
