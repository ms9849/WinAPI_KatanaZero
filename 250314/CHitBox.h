#pragma once
#include "Define.h"
#include "CObj.h"

class CHitBox : public CObj
{
public:
	/*
	Grunt -> 랄프
	Pomp -> 칼잽이
	HeadHunter -> 보스
	*/
	enum CREATOR { PLAYER, GRUNT, POMP, HEADHUNTER };
public:
	CHitBox();
	CHitBox(CREATOR _eCreator, CObj* _pTarget , float _fTargetAngle) { 
		m_eCreator = _eCreator;  
		m_pTarget = _pTarget;
		m_fTargetAngle = _fTargetAngle;
	}
	virtual ~CHitBox();

public:
	void Set_Hit(bool _bFlag) { m_bHit = _bFlag; }
	bool Get_Hit() { return m_bHit; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	CObj* Get_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	float Get_TargetAngle() { return m_fTargetAngle; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	virtual void	HitActor(CObj* HitActor, HITPOINT _HitInfo) {}
	virtual void	HitActor(CMap* HitActor, HITPOINT _HitInfo) {}
	void HitActor(CObj* HitActor) {};
private:
	bool m_bHit;
	CREATOR m_eCreator;
	CObj* m_pTarget; // 위치를 늘 추적해야 한다.. 
	float m_fDist; 
	float m_fTargetAngle; // 타겟과의 각도 차이
	// 회전용 포인트
	POINT		m_tPoint[3];
};

