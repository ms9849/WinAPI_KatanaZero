#pragma once
#include "CLine.h"

class CHeadHunter;

class CMovingLaser : public CLine
{
public:
	enum DIRECTION { LEFT, RIGHT };
public:
	CMovingLaser();
	CMovingLaser(CObj* _pTarget, DIRECTION _eDir, MAP_OBJ_TYPE _eType) : CLine(_eType), 
		m_pTarget(_pTarget), m_eDir(_eDir), m_fColorTime(GetTickCount64()), m_iColor(0)
	{
		if (_eDir == LEFT)
			m_fAngle = 225;
		else
			m_fAngle = 315;

		m_fLaserTime = 1300.f;
		m_fStartTime = GetTickCount64();
	}
	~CMovingLaser();

public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

private:
	ULONGLONG m_fLaserTime;
	ULONGLONG m_fStartTime;
	CObj* m_pTarget; // «ÏµÂ«Â≈Õ ¡¬«• πﬁæ∆æﬂ«‘.
	DIRECTION m_eDir;
	float m_fAngle; // ¿Ã∞« ππ §∑§∑.

	ULONGLONG m_fColorTime;
	int m_iColor;
};

