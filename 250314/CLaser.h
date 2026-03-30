#pragma once
#include "CLine.h"

class CLaser : public CLine
{
public:
	CLaser();
	CLaser(LINEPOINT _tLeft, LINEPOINT _tRight) : CLine(_tLeft, _tRight), m_fLaserTime(GetTickCount64()) {}
	CLaser(LINEPOINT _tLeft, LINEPOINT _tRight, MAP_OBJ_TYPE _type) : CLine(_tLeft, _tRight, _type), m_fLaserTime(GetTickCount64()) {} // 라인은 Ground, Ceiling 2종류로 나뉠 것.
	~CLaser();

public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

private:
	ULONGLONG m_fLaserTime;
	ULONGLONG m_fLaserLife = 100.f;
};

