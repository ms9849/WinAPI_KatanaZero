#pragma once
#include "CMap.h"
#include "Define.h"

class CLine : public CMap
{
public:
	CLine();
	CLine(MAP_OBJ_TYPE _eType) : CMap(_eType) {}
	CLine(LINEPOINT _tLeft, LINEPOINT _tRight);
	CLine(LINEPOINT _tLeft, LINEPOINT _tRight, MAP_OBJ_TYPE _type); // 라인은 Ground, Ceiling 2종류로 나뉠 것.
	~CLine();

public:
	const LINEPOINT& Get_Left() { return m_tLeft; }
	const LINEPOINT& Get_Right() { return m_tRight; }
	void Set_Left(LINEPOINT _tLeft) { m_tLeft = _tLeft; }
	void Set_Right(LINEPOINT _tRight) { m_tRight = _tRight; }
	
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

protected:
	LINEPOINT m_tLeft;
	LINEPOINT m_tRight;
	//Left는 무조건 Right보다 왼쪽에 있어야함 말그대로.. 그래야 오류가 없다.
};

