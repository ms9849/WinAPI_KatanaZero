#include "pch.h"
#include "CObjMgr.h"
#include "CGrunt.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "CRectMgr.h"
#include "CCollisionMgr.h"
#include "CEffectMgr.h"
#include "CSlashEffect.h"
#include "CBullet.h"
#include "CStainEffect.h"
#include "CItem.h"
CGrunt::CGrunt() : m_ePreMotion(MOTION_END), m_eCurMotion(RIGHT_WALK), 
m_bTargetFound(false), m_pTarget(nullptr), m_pTargetline(nullptr)
{
}

CGrunt::~CGrunt()
{
    Release();
}

void CGrunt::Initialize()
{
    m_fSpeed = 3.f;
    m_tInfo.fCX = 60;
    m_tInfo.fCY = 78;
    m_tSpriteInfo = m_tInfo;
    m_fTime = 0.f;
    m_fVelocity = 15.f;
    m_bJump = false;
    m_bWallHit = false;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Grunt/enemy_grunt_idle_8x2.bmp", L"Grunt_Idle");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Grunt/enemy_grunt_walk_10x2.bmp", L"Grunt_Walk");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Grunt/enemy_grunt_run_10x2.bmp", L"Grunt_Run");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Grunt/enemy_grunt_attack_7x2.bmp", L"Grunt_Attack");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Grunt/enemy_grunt_hurt_15x2.bmp", L"Grunt_Hurt");

    m_pFrameKey = L"Grunt_Walk";
    m_tFrame.iStart = 0;
    m_tFrame.iEnd = 9;
    m_tFrame.iMotion = 0;
    m_tFrame.dwTime = GetTickCount64();
    m_tFrame.dwFrameSpeed = 75.f; //추후 time 변수 추가

    m_pTarget = CObjMgr::Get_Instance()->Get_Player();
    m_fAttackDelayTime = GetTickCount64();

}

int CGrunt::Update()
{
    __super::Update_Rect();
    return NOEVENT;
}

void CGrunt::Late_Update()
{
    if (m_pFrameKey == L"Grunt_Hurt" && (isAnimationEnd()))
    {
        m_tInfo.fY += 6.5f;
        __super::Update_Sprite();
        return;
    }

    if (m_pFrameKey == L"Grunt_Hurt" && m_bJump == true)
    {
        m_tInfo.fY -= m_fVelocity * sinf(m_fAttackAngle * PI / 180.f) * m_fTime - 0.5 * 9.8f * m_fTime * m_fTime > -6.5f ? (m_fVelocity * sinf(m_fAttackAngle * PI / 180.f) * m_fTime - 0.5 * 9.8f * m_fTime * m_fTime) * g_fSlowFactor : -6.5f * g_fSlowFactor;
        
        if(!m_bWallHit)
            m_tInfo.fX += m_fVelocity * cosf(m_fAttackAngle * PI / 180.f) * g_fSlowFactor;

        m_fTime += 0.14f * g_fSlowFactor;
    }

    if (m_pFrameKey != L"Grunt_Hurt")
    {
        Calc_TargetDist();
        Move();
        Attack();
    }

    __super::Update_Frame();
    __super::Update_Sprite();
    
    Fall();
    Motion_Change();
}

void CGrunt::Render(HDC hDC)
{
    int		iScrollX = (INT)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (INT)CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
   
    GdiTransparentBlt(hDC,/// 복사 받을 dc
        m_tSpriteInfo.fX - m_tSpriteInfo.fCX / 2.f + iScrollX,		// 복사 받을 위치 좌표 left
        m_tSpriteInfo.fY - m_tSpriteInfo.fCY / 2.f + iScrollY,					// 복사 받을 위치 좌표 top
        (int)m_tSpriteInfo.fCX,				// 복사 받을 가로 사이즈
        (int)m_tSpriteInfo.fCY,				// 복사 받을 세로 사이즈
        hMemDC,							// 복사할 이미지 dc
        m_tFrame.iStart * (int)m_tSpriteInfo.fCX,
        m_tFrame.iMotion * (int)m_tSpriteInfo.fCY,							// 복사할 이미지의 left, top
        (int)m_tSpriteInfo.fCX,				// 복사할 이미지의 가로
        (int)m_tSpriteInfo.fCY,				// 복사할 이미지의 세로
        RGB(255, 0, 255));


    //Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
}

void CGrunt::Release()
{
}

void CGrunt::HitActor(CObj* HitActor, HITPOINT _HitInfo)
{
}

void CGrunt::HitActor(CMap* HitActor, HITPOINT _HitInfo)
{
	if (HitActor->GetObjType() == TYPE_GROUND) // 선 충돌.
	{
        if(nullptr == m_pTargetline)
            m_pTargetline = HitActor;

        if (m_fTime >= 3.8f)
        {
            m_bJump = false;
            m_fTime = 0.f;
        }
	}

	if (HitActor->GetObjType() == TYPE_WALL || HitActor->GetObjType() == TYPE_MONSTER_WALL) // 렉트 벽 충돌
	{
		if (_HitInfo.eDirection == DIRECTION::DIR_DOWN)
		{
            m_bJump = true;
            m_fTime = 3.9f;
		}

		if (_HitInfo.eDirection == DIRECTION::DIR_UP)
		{
		}

		if (_HitInfo.eDirection == DIRECTION::DIR_LEFT)
		{
            m_tInfo.fX += fabsf(_HitInfo.fX);

            if(m_pFrameKey != L"Grunt_Hurt")
                m_eCurMotion = RIGHT_WALK;

            if (m_bDead)
                m_bWallHit = true;
            //m_pFrameKey = L"Grunt_Walk";
            m_tInfo.fX += m_fSpeed;
		}

		if (_HitInfo.eDirection == DIRECTION::DIR_RIGHT)
		{
            m_tInfo.fX -= fabsf(_HitInfo.fX);

            if (m_pFrameKey != L"Grunt_Hurt")
                m_eCurMotion = LEFT_WALK;

            if (m_bDead)
                m_bWallHit = true;
            //m_pFrameKey = L"Grunt_Walk";
            m_tInfo.fX -= m_fSpeed;
		}
	}

}

void CGrunt::HitActor(CObj* HitActor)
{
    if (m_bDead) return;

    if (HitActor->Get_Type() == BULLET && !dynamic_cast<CBullet*>(HitActor)->Get_Parried())
        return;
    else if (HitActor->Get_Type() == ITEM && (dynamic_cast<CItem*>(HitActor)->Get_Throw() == false || HitActor->Get_Dead() == true))
        return;
    else if (HitActor->Get_Type() == BULLET && dynamic_cast<CBullet*>(HitActor)->Get_Parried() && HitActor->Get_Dead() == true)
        return;
    else if (HitActor->Get_Type() == OBJ && dynamic_cast<CHitBox*>(HitActor)->Get_Hit() == true)
        return;

    if (HitActor->Get_Type() == BULLET && dynamic_cast<CBullet*>(HitActor)->Get_Parried())
        m_fAttackAngle = HitActor->Get_Info()->fX < m_tInfo.fX ? 135 : 45;

    else if (HitActor->Get_Type() == OBJ)
    {
        m_fAttackAngle = dynamic_cast<CHitBox*>(HitActor)->Get_TargetAngle();
        dynamic_cast<CHitBox*>(HitActor)->Set_Hit(true);
    }

    else if (HitActor->Get_Type() == ITEM && dynamic_cast<CItem*>(HitActor)->Get_Throw() == true)
    {
        m_fAttackAngle = dynamic_cast<CItem*>(HitActor)->Get_ThrowAngle();
        HitActor->Set_Dead(true);
    }
       

    if (!m_bDead)
    {
        if (m_tFrame.iMotion == 1)
            m_eCurMotion = LEFT_HURT;

        else
            m_eCurMotion = RIGHT_HURT;

        if (m_pFrameKey != L"Grunt_Hurt")
            m_bJump = true;

        if ((HitActor->Get_Type() == OBJ || HitActor->Get_Type() == ITEM) && !m_bDead)
            CSoundMgr::Get_Instance()->PlaySound(L"monster_death_sword.wav", SOUND_EFFECT, g_fVolume);
        else if (HitActor->Get_Type() == BULLET && !m_bDead)
        {
            CSoundMgr::Get_Instance()->PlaySound(L"monster_death_bullet.wav", SOUND_EFFECT, g_fVolume);
            HitActor->Set_Dead(true);
        }

        //피범벅 이펙트
        CEffect* pEffect = nullptr;
        if (m_fAttackAngle > 90 && m_fAttackAngle < 270)
        {
            if (HitActor->Get_Type() == OBJ)
                pEffect = new CFollowEffect(m_tInfo.fX, m_tInfo.fY, L"Right_Effect_Blood", this);
            else
                pEffect = new CFollowEffect(m_tInfo.fX, m_tInfo.fY, L"Left_Effect_Blood", this);
        }
        else
        {
            if (HitActor->Get_Type() == OBJ)
                pEffect = new CFollowEffect(m_tInfo.fX, m_tInfo.fY, L"Left_Effect_Blood", this);
            else
                pEffect = new CFollowEffect(m_tInfo.fX, m_tInfo.fY, L"Right_Effect_Blood", this);
        }

        pEffect->Initialize();
        CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);

        //핏자국 이펙트
        pEffect = new CStainEffect(m_tInfo.fX, m_tInfo.fY, L"Blood_Stain", 180 + m_fAttackAngle);
        pEffect->Initialize();
        CEffectMgr::Get_Instance()->Add_Effect(pEffect);

        //슬래시 이펙트 추가
        if (HitActor->Get_Type() == OBJ)
            pEffect = new CSlashEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Enemy_Slash", dynamic_cast<CHitBox*>(HitActor)->Get_TargetAngle());

        else if (HitActor->Get_Type() == BULLET)
            pEffect = new CSlashEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Enemy_Slash", dynamic_cast<CBullet*>(HitActor)->Get_Angle());
        
        else if (HitActor->Get_Type() == ITEM)
            pEffect = new CSlashEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Enemy_Slash", dynamic_cast<CItem*>(HitActor)->Get_ThrowAngle());
        pEffect->Initialize();
        CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);

        //카메라 쉐낏
        if (g_bSlowMod)
            CScrollMgr::Get_Instance()->Shake(10, 200); //ms단위로 시간 입력.
        else
            CScrollMgr::Get_Instance()->Shake(10, 200); //ms단위로 시간 입력.
        m_bDead = true; // 여기서 몬스터는 사망판정을 받음. 충돌처리니까 맨 마지막..
    }
    Motion_Change();
}

void CGrunt::Fall()
{
    if (nullptr == m_pTargetline)
        m_tInfo.fY += 3.0f;

    else if (m_pFrameKey != L"Grunt_Hurt")
    {
        m_tInfo.fY += 8.0f;
    }
}

void CGrunt::Calc_TargetDist()
{
    if (nullptr != m_pTarget)
    {
        float fDistX = m_pTarget->Get_Info()->fX - m_tInfo.fX;
        float fDistY = m_pTarget->Get_Info()->fY - m_tInfo.fY;
        m_fTargetDist = sqrtf(fDistX * fDistX + fDistY * fDistY);

        if (m_fTargetDist <= 10)
            m_bTargetFound = true;
    }
}

//현재는 walk, idle 뿐임.
void CGrunt::Move()
{
    if (nullptr != m_pTargetline && m_pFrameKey != L"Grunt_Attack")
    {
        if (!m_bTargetFound)
        {
            if (dynamic_cast<CLine*>(m_pTargetline)->Get_Left().fX + 25 < m_tInfo.fX &&
                dynamic_cast<CLine*>(m_pTargetline)->Get_Right().fX - 25 > m_tInfo.fX)
            {
                if (m_eCurMotion == LEFT_WALK)
                    m_tInfo.fX -= m_fSpeed * g_fSlowFactor;

                else if (m_eCurMotion == RIGHT_WALK)
                    m_tInfo.fX += m_fSpeed * g_fSlowFactor;
            }

            //달리다가 끝에 도달했을 때 IDLE
            else if (m_eCurMotion == LEFT_WALK || m_eCurMotion == RIGHT_WALK)
                m_eCurMotion = IDLE;

            // 되돌아 움직이기.
            else
            {
                if (m_tFrame.iMotion == 0 && m_tFrame.iStart == m_tFrame.iEnd)
                {
                    m_eCurMotion = LEFT_WALK;
                    m_tInfo.fX -= m_fSpeed;
                }

                else if (m_tFrame.iMotion == 1 && m_tFrame.iStart == 0)
                {
                    m_eCurMotion = RIGHT_WALK;
                    m_tInfo.fX += m_fSpeed;
                }
            }
        }
        // 플레이어 발견시 뛰어다니기
        else if (m_bTargetFound)
        {
            if (m_pTarget->Get_Info()->fX + 20 < m_tInfo.fX)
            {
                m_tInfo.fX -= m_fSpeed * g_fSlowFactor * 1.5f;
                m_eCurMotion = LEFT_RUN;
            }

            else if (m_pTarget->Get_Info()->fX - 20 > m_tInfo.fX)
            {
                m_tInfo.fX += m_fSpeed * g_fSlowFactor * 1.5f;
                m_eCurMotion = RIGHT_RUN;
            }

            else // ㅇㅇ
               m_eCurMotion = IDLE;
        }
    }
}

void CGrunt::Attack()
{
    if (m_pFrameKey == L"Grunt_Attack" && ((m_tFrame.iMotion == 0 && m_tFrame.iStart == 3) ||
        (m_tFrame.iMotion == 1 && m_tFrame.iStart == 3))) // 공격 중 히트박스 삽입
    {
        CObj* pHitBox = new CHitBox(CHitBox::GRUNT, this, m_pTarget->Get_Info()->fX < m_tInfo.fX ? 180.f : 0.f);
        pHitBox->Initialize();
        CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER_HITBOX, pHitBox);
    }

    else if (m_pFrameKey == L"Grunt_Attack" && ((m_tFrame.iMotion == 0 && m_tFrame.iStart == m_tFrame.iEnd) ||
        (m_tFrame.iMotion == 1 && m_tFrame.iStart == 0))) // 공격 끝
    {
        m_eCurMotion = IDLE;
    }

    if (m_bTargetFound && m_fTargetDist <= 50 && GetTickCount64() - m_fAttackDelayTime > 1500 )
    {

        if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
            m_eCurMotion = LEFT_ATTACK;
        else
            m_eCurMotion = RIGHT_ATTACK;

        m_fAttackDelayTime = GetTickCount64();
    }
}

void CGrunt::Motion_Change()
{
	if (m_ePreMotion != m_eCurMotion)
	{
		switch (m_eCurMotion)
		{
		case IDLE:
			if (m_ePreMotion == LEFT_RUN || m_ePreMotion == LEFT_WALK ||
				m_ePreMotion == LEFT_ATTACK )
			{
				m_tFrame.iStart = 7;
				m_tFrame.iEnd = 7;
				m_tFrame.iMotion = 1;
			}
			else
			{
				m_tFrame.iStart = 0;
				m_tFrame.iEnd = 7;
				m_tFrame.iMotion = 0;
			}
			m_tSpriteInfo.fCX = 60;
			m_tSpriteInfo.fCY = 72;

            m_pFrameKey = L"Grunt_Idle";
			m_tFrame.dwTime = GetTickCount64();
			m_tFrame.dwFrameSpeed = 75;
			break;

        case LEFT_WALK:
            m_tFrame.iStart = 9;
            m_tFrame.iEnd = 9;
            m_tFrame.iMotion = 1;

            m_pFrameKey = L"Grunt_Walk";
            m_tSpriteInfo.fCX = 64;
            m_tSpriteInfo.fCY = 80;
            break;
        case RIGHT_WALK:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 9;
            m_tFrame.iMotion = 0;

            m_pFrameKey = L"Grunt_Walk";
            m_tSpriteInfo.fCX = 64;
            m_tSpriteInfo.fCY = 80;
            break;

        case LEFT_RUN:
            m_tFrame.iStart = 9;
            m_tFrame.iEnd = 9;
            m_tFrame.iMotion = 1;

            m_pFrameKey = L"Grunt_Run";
            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 78;
            break;

        case RIGHT_RUN:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 9;
            m_tFrame.iMotion = 0;

            m_pFrameKey = L"Grunt_Run";
            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 78;
            break;

        case LEFT_ATTACK:
            m_tFrame.iStart = 6;
            m_tFrame.iEnd = 6;
            m_tFrame.iMotion = 1;

            m_pFrameKey = L"Grunt_Attack";
            m_tSpriteInfo.fCX = 88;
            m_tSpriteInfo.fCY = 84;
            break;

        case RIGHT_ATTACK:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 6;
            m_tFrame.iMotion = 0;

            m_pFrameKey = L"Grunt_Attack";
            m_tSpriteInfo.fCX = 88;
            m_tSpriteInfo.fCY = 84;
            break;

        case LEFT_HURT:
            m_tFrame.iStart = 14;
            m_tFrame.iEnd = 14;
            m_tFrame.iMotion = 1;

            m_pFrameKey = L"Grunt_Hurt";
            m_tSpriteInfo.fCX = 112;
            m_tSpriteInfo.fCY = 82;
            break;
        case RIGHT_HURT:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 14;
            m_tFrame.iMotion = 0;

            m_pFrameKey = L"Grunt_Hurt";
            m_tSpriteInfo.fCX = 112;
            m_tSpriteInfo.fCY = 82;
            break;
		}
        m_ePreMotion = m_eCurMotion;
	}
}