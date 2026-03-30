#pragma once
#include "CObj.h"
#include "CMap.h"

class CRect : public CMap
{
public:
	CRect();
	CRect(RECT _tInfo) { 
		m_tRect = _tInfo; 
		m_tInfo.fX = (m_tRect.left + m_tRect.right) / 2.f;
		m_tInfo.fY = (m_tRect.top + m_tRect.bottom) / 2.f;
		m_tInfo.fCX = m_tRect.right - m_tRect.left;
		m_tInfo.fCY = m_tRect.top - m_tRect.bottom;
	}

	CRect(RECT _tInfo, MAP_OBJ_TYPE _Type)
	{ 
		m_tRect = _tInfo;
		m_tInfo.fX = (m_tRect.left + m_tRect.right) / 2.f;
		m_tInfo.fY = (m_tRect.top + m_tRect.bottom) / 2.f;
		m_tInfo.fCX = m_tRect.right - m_tRect.left;
		m_tInfo.fCY = m_tRect.top - m_tRect.bottom;
		m_eType = _Type;
	}
	~CRect();

public:
	void Set_Rect(RECT _tRect) { m_tRect = _tRect; }

public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

	const RECT* Get_Rect() { return &m_tRect; }

protected:
};

