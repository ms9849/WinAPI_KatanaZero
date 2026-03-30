#pragma once
#include "CObj.h"
#include "Define.h"
#include "CHitBox.h"
#include "CMap.h"
#include "CLine.h"
#include "CBullet.h"
//주먹쓰는 랄프같은 놈임.
class CGrunt : public CObj
{
public:
	enum MSTATE {
		IDLE, 
		LEFT_WALK, RIGHT_WALK,
		LEFT_RUN, RIGHT_RUN,
		LEFT_ATTACK, RIGHT_ATTACK,
		LEFT_HURT, RIGHT_HURT,
		MOTION_END
	};
public:
	CGrunt();
	~CGrunt();

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
	void Attack();
	void Fall();
	void Motion_Change();

public:
	void HitActor(CObj* HitActor, HITPOINT _HitInfo) override;
	void HitActor(CMap* HitActor, HITPOINT _HitInfo) override;
	void HitActor(CObj* HitActor) override;
private:
	MSTATE m_eCurMotion;
	MSTATE m_ePreMotion;

	float m_fTargetDist; // 타겟과의 거리, 얜 무조건 플레이어긴 한데..
	float m_fTime;
	float m_fVelocity;
	float m_fAttackAngle;

	bool m_bWallHit;
	bool m_bJump;
	bool m_bTargetFound;
	bool m_bOnGround;
	CObj* m_pTarget;
	CMap* m_pTargetline; // 몬스터가 계속 밟고 있어야 하는 선.
	ULONGLONG m_fAttackDelayTime;
};

