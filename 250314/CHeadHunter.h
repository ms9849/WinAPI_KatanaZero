#pragma once
#include "CObj.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CLaser.h"
#include "CMovingLaser.h"

class CHeadHunter : public CObj
{
public:
	enum MSTATE {
		IDLE,
		LEFT_ROLL, RIGHT_ROLL,
		LEFT_DRAW_GUN, RIGHT_DRAW_GUN,
		LEFT_AIM, RIGHT_AIM,
		LEFT_PRE_RUSH, RIGHT_PRE_RUSH,
		LEFT_RUSH, RIGHT_RUSH,
		LEFT_WALLGRAP, RIGHT_WALLGRAP,
		LEFT_SWEEP, RIGHT_SWEEP,
		LEFT_TELEPORT, RIGHT_TELEPORT,
		LEFT_DEATH_JUMP, RIGHT_DEATH_JUMP,
		LEFT_DEATH_GROUND, RIGHT_DEATH_GROUND,
		MOTION_END
	};

	enum PATTERN {
		NONE,
		DODGE,
		GROUND_LASER,
		RUSH,
		SWEEP,
		TELEPORT,
		DEATH,
		PATTERN_END
	};

public:
	CHeadHunter();
	virtual ~CHeadHunter();

public:
	OBJTYPE Get_Type() const override { return OBJTYPE::MONSTER; }
	void Set_Target(CPlayer* _pTarget) { m_pTarget = _pTarget; }
	MSTATE Get_Motion() { return m_eCurMotion; }
	bool Get_Visible() { return m_bVisible; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Calc_Target();
	void Update_Pattern();
	void Motion_Change();

public:
	void HitActor(CObj* HitActor, HITPOINT _HitInfo) override;
	void HitActor(CMap* HitActor, HITPOINT _HitInfo) override;
	void HitActor(CObj* HitActor) override;

public:
	//패턴은 한번에 하나밖에 사용 못함.
	// 땅 유도 레이저
	void Ground_Laser();
	// 회피
	void Dodge();
	// 러쉬
	void Rush();
	// 벽 짚은 후 레이저 발광 
	void Raser_Sweep();
	// 텔포
	void Teleport();
	// 죽었을때 뛰어 오르는거
	void Death_Bound();

private:
	MSTATE m_eCurMotion;
	MSTATE m_ePreMotion;
	PATTERN m_ePattern;
	PATTERN m_ePrePattern;

	bool m_bWallGrap;
	bool m_bJump;
	float m_fTime;

	float m_fTargetDist; // 타겟과의 거리, 얜 무조건 플레이어긴 한데..
	float m_fTargetAngle; // 타겟과의 각도

	float m_fVelocity;
	float m_fAttackAngle;

	CPlayer* m_pTarget;
	ULONGLONG m_fAttackDelayTime;

	//땅에서 쏘는 레이저
	ULONGLONG m_fLaserDelayTime;
	ULONGLONG m_fLaserShootTime;
	bool m_bLaserShoot;

	ULONGLONG m_fRushDelayTime;
	float m_fRushAngle;
	
	bool m_bIsHitCeil;
	bool m_bSweepLaser;
	bool m_bVisible;
	POINT		m_tCheckPoint[2]; // 순간이동 해서 레이저 및 총알 쏠 곳?
};

