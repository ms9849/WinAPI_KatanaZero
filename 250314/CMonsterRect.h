#pragma once
#include "CMap.h"
#include "CRect.h"

class CMonsterRect : public CRect
{
public:
	CMonsterRect();
	CMonsterRect(RECT _tInfo): CRect(_tInfo) {}
	CMonsterRect(RECT _tInfo, MAP_OBJ_TYPE _Type) : CRect(_tInfo, _Type) {}
	virtual ~CMonsterRect();

public:
	void Set_Rect(RECT _tRect) { m_tRect = _tRect; }

public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

	const RECT* Get_Rect() { return &m_tRect; }
};

