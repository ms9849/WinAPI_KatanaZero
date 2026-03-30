#pragma once
#include "Define.h"
#include "CObj.h"
#include "CMap.h"
#include "CHitBox.h"
#include "CBullet.h"

//총 쏘는 정장 대머리
class CBold : public CObj
{
public:
	enum MSTATE {
		IDLE,
		LEFT_WALK, RIGHT_WALK,
		LEFT_RUN, RIGHT_RUN,
		LEFT_AIM, RIGHT_AIM,
		LEFT_HURT, RIGHT_HURT,
		MOTION_END
	};

public:
	CBold();
	~CBold();

public:
	OBJTYPE Get_Type() const override { return OBJTYPE::MONSTER; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_Targetline(CMap* _pLine) { m_pTargetline = _pLine; }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Calc_TargetDist();
	void Move();
	void Aim();
	void Fall();
	void Motion_Change();

public:
	void HitActor(CObj* HitActor, HITPOINT _HitInfo) override;
	void HitActor(CMap* HitActor, HITPOINT _HitInfo) override;
	void HitActor(CObj* HitActor) override;

private:
	MSTATE m_eCurMotion;
	MSTATE m_ePreMotion;

	float m_fTargetAngle;
	float m_fTargetDist; // 타겟과의 거리, 얜 무조건 플레이어긴 한데..
	float m_fTime;
	float m_fVelocity;
	float m_fAttackAngle;

	bool m_bWallHit;
	bool m_bJump;
	bool m_bTargetFound;
	bool m_bOnGround;
	bool m_bShoot;

	CObj* m_pTarget;
	CMap* m_pTargetline; // 몬스터가 계속 밟고 있어야 하는 선.
	ULONGLONG m_fAttackDelayTime;
	ULONGLONG m_fAimDelayTime;
	ULONGLONG m_fAfterShootDelayTime;
};

