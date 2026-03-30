#pragma once
#include "CObj.h"
#include "CRestartUI.h"
#include "CHitBox.h"
#include "Define.h"
#include "CItem.h"

class CPlayer :   public CObj
{
public:
	enum MSTATE { IDLE, DANCE, PLAYSONG, // 완, playsong은 엔딩에서만 재생 예정
		LEFT_RUN, RIGHT_RUN, //완
		LEFT_ROLL, RIGHT_ROLL, // 완
		LEFT_JUMP, RIGHT_JUMP, // 완
		LEFT_FALL, RIGHT_FALL, // 완.
		LEFT_ATTACK, RIGHT_ATTACK, // 하는 중. 공격이 끝나면 이전 상태로 돌아가야 하..긴하는데?...
		LEFT_RUN_TO_IDLE, RIGHT_RUN_TO_IDLE, // 완
		LEFT_IDLE_TO_RUN, RIGHT_IDLE_TO_RUN, // 완
		LEFT_WALLGRAP, RIGHT_WALLGRAP, // 완
		LEFT_WALLJUMP, RIGHT_WALLJUMP, // 완
		LEFT_DEATH_JUMP, RIGHT_DEATH_JUMP,
		LEFT_DEATH_GROUND, RIGHT_DEATH_GROUND,
		DRAW_SWORD,
		MOTION_END
	};

public:
	CPlayer();
	virtual ~CPlayer();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	CObj* Get_Hand() { return m_pHand; }
	OBJTYPE Get_Type() const override { return OBJTYPE::PLAYER; }
	bool Get_Collided() { return m_bCollided; }
	float Get_AttackAngle() { return m_fAttackAngle; }
	int Get_SlowGauge() { return m_iSlowGauge; }
	MSTATE Get_Motion() { return m_eCurMotion; }
	void Create_Trail();
private:
	void Death();
	void Key_Input();
	void Run_to_Idle();
	void Idle_to_Run();
	void Attack();
	void ShiShiSonSon();
	void Roll();
	void Jump();
	void Fall();
	void Offset();
	void Motion_Change();

public:
	void HitActor(CObj* HitActor, HITPOINT _HitInfo) override;
	void HitActor(CMap* HitActor, HITPOINT _HitInfo) override;
	void HitActor(CObj* HitActor) override;

private:
	MSTATE m_eCurMotion;
	MSTATE m_ePreMotion;
	CMap* m_pLine; // 대각선이면 타게 하자.
	bool m_bJump;
	float m_fMaxJumpSpeed;
	float m_fMaxWallJumpSpeed;
	float m_fJumpSpeed;
	float m_fWallFallSpeed;
	float m_fTime;
	float m_fDeathTime; 
	float m_fAttackAngle;

	bool m_bDeadFlag;
	bool m_bFall;
	bool m_bFallFlag; // 낙하 시 모션 변경 ( 점프 중 1회만 가능하도록. )
	bool m_bAttackFloat; // 공격시 뜨는거 체크하기 위한 flag
	bool m_bCollided;
	bool m_bWallGrap;
	bool m_bOnGround;
	bool m_bInvincible;

	int m_iAttackCount;
	int m_iSlowGauge;
	int m_iMaxSlowGauge;
	float m_fRollSpeed;
	float m_fAttackTime;
	float m_fAttackVelocity;

	ULONGLONG m_fRollDelayTime;
	ULONGLONG m_fAttackDelayTime;
	ULONGLONG m_fRunSoundDelayTime;
	//sprite Rect와 실제 Rect의 분리는 성공했음.

	CObj* m_pHand; // 손에 아이템 쥐게 하기
};

