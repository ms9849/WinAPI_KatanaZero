#pragma once
#include "Define.h"
#include "CObj.h"

class CMap : public CObj
{
public:
	CMap();
	CMap(MAP_OBJ_TYPE _eType) : m_eType(_eType) {}
	virtual ~CMap();
	MAP_OBJ_TYPE GetObjType() const { return m_eType; }

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void	HitActor(CObj* HitActor, HITPOINT _HitInfo);
	void	HitActor(CMap* HitActor, HITPOINT _HitInfo);

protected:
	MAP_OBJ_TYPE m_eType;

};

