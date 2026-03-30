#pragma once
#include "CObj.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CLaser.h"
#include "CMovingLaser.h"
#include "CBullet.h"
#include "CAbstractFactory.h"
#include "CHead.h"
#include "CStainEffect.h"
#include "CSlashEffect.h"

class CHeadHunter_2 : public CObj
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
		LEFT_BULLET_SWEEP, RIGHT_BULLET_SWEEP,
		LEFT_LASER_SWEEP, RIGHT_LASER_SWEEP,
		LEFT_TELEPORT, RIGHT_TELEPORT,
		LEFT_DEATH_JUMP, RIGHT_DEATH_JUMP,
		LEFT_DEATH_GROUND, RIGHT_DEATH_GROUND,
		LEFT_NO_HEAD, RIGHT_NO_HEAD,
		AERIAL_ATTACK_IN, AERIAL_ATTACK_OUT,
		MOTION_END
	};

	enum PATTERN {
		NONE,
		DODGE,
		GROUND_LASER,
		GROUND_BULLET,
		RUSH,
		BULLET_SWEEP,
		AERIAL_LASER,
		AERIAL_BULLET,
		AERIAL_SWEEP,
		TELEPORT,
		DEATH,
		PATTERN_END
	};

	enum AERIAL_DIR {
		LEFT,
		RIGHT
	};

public:
	CHeadHunter_2();
	virtual ~CHeadHunter_2();
public:
	OBJTYPE Get_Type() const override { return OBJTYPE::MONSTER; }
	void Set_Target(CPlayer* _pTarget) { m_pTarget = _pTarget; }
	MSTATE Get_Motion() { return m_eCurMotion; }

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
	void Move();

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
	void Bullet_Sweep();

	// 벽 짚은 후 레이저 발광 
	void Aerial_Bullet();
	void Aerial_Laser();
	// 텔포 (들어갈진 모름)
	//
	void Aerial_Sweep();

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
	int m_iDeathCount;
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

	//돌진
	ULONGLONG m_fRushDelayTime; //넣을지 ㅁㄹ
	float m_fRushAngle; // 넣을지 ㅁㄹ 일단 가지고 있음

	//총 휩쓸기
	float m_fBulletAngle;
	ULONGLONG m_fBulletTimer;

	//공중 공격
	int m_iAerialCount;
	ULONGLONG m_fFloatTimer;
	bool m_bIsHitCeil;
	bool m_bBulletSweep;
	bool m_bHeadCut;
	AERIAL_DIR m_eAerialDir;

	POINT		m_tAerialSweepCheckPoint[2]; // 공중 3포인트. 레이저나 총알 발사하게 해야 한다.
	POINT		m_tAerialCheckPoint[3]; // 공중 3포인트. 레이저나 총알 발사하게 해야 한다.
	POINT		m_tGroundCheckPoint[2]; // 지상 2포인트. 마찬가지로 레이저나 총알을 발사해야 한다.
};

