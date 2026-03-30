#pragma once
#include "CObj.h"
#include "CMap.h"
#include "CBullet.h"
// 아이템이라고는 했는데 식칼뿐이긴 함..
class CItem : public CObj
{
public:
	CItem();
	virtual ~CItem();

public:
	OBJTYPE Get_Type() const override { return OBJTYPE::ITEM; }
	bool Get_Parried() { return m_bParried; }

	bool Get_Throw() { return m_bThrowing; }
	void Set_Throw(bool _bFlag) { m_bThrowing = _bFlag; }
	float Get_ThrowAngle() { return m_fThrowingAngle; }
	void Set_ThrowAngle(float _fAngle) { m_fThrowingAngle = _fAngle; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void HitActor(CObj* HitActor, HITPOINT _HitInfo) {}

	//맵과의 충돌
	void HitActor(CMap* HitActor, HITPOINT _HitInfo);
	//히트박스, 몬스터와의 충돌
	void HitActor(CObj* HitActor);

private:
	bool m_bThrowing;
	bool m_bParried; // 걍 m_bDead로 하면 안되나? ㅁㄹ
	float m_fThrowingAngle;
	Image* m_pImage;
};

