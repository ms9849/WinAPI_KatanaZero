#include "pch.h"
#include "CBold.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "CEffectMgr.h"
#include "CSlashEffect.h"
#include "CStainEffect.h"
#include "CItem.h"

CBold::CBold() : m_ePreMotion(MOTION_END), m_eCurMotion(RIGHT_WALK),
m_bTargetFound(false), m_pTarget(nullptr), m_pTargetline(nullptr)
{
}

CBold::~CBold()
{
    Release();
}

void CBold::Initialize()
{
    m_fSpeed = 3.f;
    m_tInfo.fCX = 72;
    m_tInfo.fCY = 70;
    m_tSpriteInfo = m_tInfo;
    m_fTime = 0.f;
    m_fVelocity = 15.f;
    m_bJump = false;
    m_bWallHit = false;
    m_bShoot = false;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/enemy_bold_idle_8x2.bmp", L"Bold_Idle");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/enemy_bold_walk_8x2.bmp", L"Bold_Walk");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/enemy_bold_run_10x2.bmp", L"Bold_Run");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/enemy_bold_aim_1x2.bmp", L"Bold_Aim");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/enemy_bold_hurt_14x2.bmp", L"Bold_Hurt");

    m_pFrameKey = L"Bold_Walk";
    m_tFrame.iStart = 0;
    m_tFrame.iEnd = 7;
    m_tFrame.iMotion = 0;
    m_tFrame.dwTime = GetTickCount64();
    m_tFrame.dwFrameSpeed = 75.f; //추후 time 변수 추가

    m_pTarget = CObjMgr::Get_Instance()->Get_Player();
    m_fAttackDelayTime = GetTickCount64();
}

int CBold::Update()
{    
    __super::Update_Rect();
    return NOEVENT;
}

void CBold::Late_Update()
{
    if (m_pFrameKey == L"Bold_Hurt" && (isAnimationEnd()))
    {
        m_tInfo.fY += 6.5f;
        __super::Update_Sprite();
        return;
    }

    if (m_pFrameKey == L"Bold_Hurt" && m_bJump == true)
    {
        m_tInfo.fY -= m_fVelocity * sinf(m_fAttackAngle * PI / 180.f) * m_fTime - 0.5 * 9.8f * m_fTime * m_fTime > -6.5f ? (m_fVelocity * sinf(m_fAttackAngle * PI / 180.f) * m_fTime - 0.5 * 9.8f * m_fTime * m_fTime) * g_fSlowFactor : -6.5f * g_fSlowFactor;
        
        if(!m_bWallHit)
            m_tInfo.fX += m_fVelocity * cosf(m_fAttackAngle * PI / 180.f) * g_fSlowFactor;

        m_fTime += 0.14f * g_fSlowFactor;
    }

    if (m_pFrameKey != L"Bold_Hurt")
    {
        Calc_TargetDist();
        Move();
        Aim();
    }

    __super::Update_Frame();
    __super::Update_Sprite();

    Fall();
    Motion_Change();
}

void CBold::Render(HDC hDC)
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
}

void CBold::Release()
{
}

void CBold::Calc_TargetDist()
{
    if (nullptr != m_pTarget)
    {
        float fDistX = m_pTarget->Get_Info()->fX - m_tInfo.fX;
        float fDistY = m_pTarget->Get_Info()->fY - m_tInfo.fY;
        m_fTargetDist = sqrtf(fDistX * fDistX + fDistY * fDistY);

        if (m_fTargetDist <= 150)
            m_bTargetFound = true;

        if (m_pTarget->Get_Info()->fY < m_tInfo.fY)
            m_fTargetAngle = acosf(fDistX / m_fTargetDist) * 180 / PI;

        else
            m_fTargetAngle = 360.f - acosf(fDistX / m_fTargetDist) * 180 / PI;
    }
}

void CBold::Move()
{
    if (nullptr != m_pTargetline && m_pFrameKey != L"Bold_Aim")
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
            if (m_pTarget->Get_Info()->fX + 300 < m_tInfo.fX)
            {
                m_tInfo.fX -= m_fSpeed * g_fSlowFactor * 1.5f;
                m_eCurMotion = LEFT_RUN;
            }

            else if (m_pTarget->Get_Info()->fX - 300 > m_tInfo.fX)
            {
                m_tInfo.fX += m_fSpeed * g_fSlowFactor * 1.5f;
                m_eCurMotion = RIGHT_RUN;
            }

            else // ㅇㅇ
                m_eCurMotion = IDLE;
        }
    }
    else if (m_pFrameKey == L"Bold_Aim")
    {
        if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
            m_eCurMotion = LEFT_AIM;

        else if (m_pTarget->Get_Info()->fX > m_tInfo.fX)
            m_eCurMotion = RIGHT_AIM;
    }
}

void CBold::Aim()
{
    //에임 대기시간 (1~2초? 끝나면 슛)
    if (m_pFrameKey == L"Bold_Aim" && ((GetTickCount64() - m_fAimDelayTime) >= 1000 ) && !m_bShoot) // 공격 중 히트박스 삽입
    {
        CObj* pBullet = new CBullet(m_tInfo.fX, m_tInfo.fY, m_fTargetAngle);
        pBullet->Initialize();
        CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
        CSoundMgr::Get_Instance()->PlaySound(L"gun_fire_1.wav", SOUND_EFFECT, g_fVolume);

        CEffect* pEffect; 
        if (m_eCurMotion == LEFT_AIM)
            pEffect = new CRemainEffect(m_tInfo.fX - 70, m_tInfo.fY, L"Left_Gun_Spark");
        else
            pEffect = new CRemainEffect(m_tInfo.fX + 70, m_tInfo.fY, L"Right_Gun_Spark");

        pEffect->Initialize();
        CEffectMgr::Get_Instance()->Add_Effect(pEffect);

        m_fAfterShootDelayTime = GetTickCount64();
        m_bShoot = true;
    }

    // 공격 끝. 이것도 100ms 정도 딜레이 주면 될듯
    else if (m_pFrameKey == L"Bold_Aim" && (GetTickCount64() - m_fAfterShootDelayTime >= 1000) && m_bShoot)
    {
        m_eCurMotion = IDLE;
        m_bShoot = false;
    }

    if (m_bTargetFound && m_fTargetDist <= 450 && (GetTickCount64() - m_fAttackDelayTime > 3000) && m_pFrameKey != L"Bold_Aim")
    {

        if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
            m_eCurMotion = LEFT_AIM;

        else
            m_eCurMotion = RIGHT_AIM;

        m_fAimDelayTime = GetTickCount64();
        m_fAttackDelayTime = GetTickCount64();
    }
}

void CBold::Fall()
{
    if (nullptr == m_pTargetline)
        m_tInfo.fY += 3.0f;

    else if (m_pFrameKey != L"Bold_Hurt")
    {
        m_tInfo.fY += 8.0f;
    }
}

void CBold::Motion_Change()
{
    if (m_ePreMotion != m_eCurMotion)
    {
        switch (m_eCurMotion)
        {
        case IDLE:
            if (m_ePreMotion == LEFT_RUN || m_ePreMotion == LEFT_WALK ||
                m_ePreMotion == LEFT_AIM)
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
            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 70;

            m_pFrameKey = L"Bold_Idle";
            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case LEFT_WALK:
            m_tFrame.iStart = 7;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 1;

            m_pFrameKey = L"Bold_Walk";
            m_tSpriteInfo.fCX = 68;
            m_tSpriteInfo.fCY = 76;
            break;
        case RIGHT_WALK:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 0;

            m_pFrameKey = L"Bold_Walk";
            m_tSpriteInfo.fCX = 68;
            m_tSpriteInfo.fCY = 76;
            break;

        case LEFT_RUN:
            m_tFrame.iStart = 9;
            m_tFrame.iEnd = 9;
            m_tFrame.iMotion = 1;

            m_pFrameKey = L"Bold_Run";
            m_tSpriteInfo.fCX = 90;
            m_tSpriteInfo.fCY = 78;
            break;
        case RIGHT_RUN:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 9;
            m_tFrame.iMotion = 0;

            m_pFrameKey = L"Bold_Run";
            m_tSpriteInfo.fCX = 90;
            m_tSpriteInfo.fCY = 78;
            break;
        case LEFT_AIM:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 1;

            m_pFrameKey = L"Bold_Aim";
            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 70;
            break;
        case RIGHT_AIM:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 0;

            m_pFrameKey = L"Bold_Aim";
            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 70;
            break;

        case LEFT_HURT:
            m_tFrame.iStart = 13;
            m_tFrame.iEnd = 13;
            m_tFrame.iMotion = 1;

            m_pFrameKey = L"Bold_Hurt";
            m_tSpriteInfo.fCX = 74;
            m_tSpriteInfo.fCY = 64;
            break;
        case RIGHT_HURT:
            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 13;
            m_tFrame.iMotion = 0;

            m_pFrameKey = L"Bold_Hurt";
            m_tSpriteInfo.fCX = 74;
            m_tSpriteInfo.fCY = 64;
            break;
            
        }
        m_ePreMotion = m_eCurMotion;
    }
}

void CBold::HitActor(CObj* HitActor, HITPOINT _HitInfo)
{
}

void CBold::HitActor(CMap* HitActor, HITPOINT _HitInfo)
{
    if (HitActor->GetObjType() == TYPE_GROUND) // 선 충돌.
    {
        if (nullptr == m_pTargetline)
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

            if (m_pFrameKey != L"Bold_Hurt")
                m_eCurMotion = RIGHT_WALK;

            if (m_bDead)
                m_bWallHit = true;
            //m_pFrameKey = L"Grunt_Walk";
            m_tInfo.fX += m_fSpeed;
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_RIGHT)
        {
            m_tInfo.fX -= fabsf(_HitInfo.fX);

            if (m_pFrameKey != L"Bold_Hurt")
                m_eCurMotion = LEFT_WALK;

            if (m_bDead)
                m_bWallHit = true;
            //m_pFrameKey = L"Grunt_Walk";
            m_tInfo.fX -= m_fSpeed;
        }
    }
}

void CBold::HitActor(CObj* HitActor)
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

    if (HitActor->Get_Type() == BULLET && dynamic_cast<CBullet*>(HitActor)->Get_Parried() && HitActor->Get_Dead() == false)
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

        if (m_pFrameKey != L"Bold_Hurt")
            m_bJump = true;

        if ((HitActor->Get_Type() == OBJ || HitActor->Get_Type() == ITEM) && !m_bDead)
            CSoundMgr::Get_Instance()->PlaySound(L"monster_death_sword.wav", SOUND_EFFECT, g_fVolume);
        else if (HitActor->Get_Type() == BULLET)
        {
            CSoundMgr::Get_Instance()->PlaySound(L"monster_death_bullet.wav", SOUND_EFFECT, g_fVolume);
            HitActor->Set_Dead(true);
        }

        //피범벅 이펙트
        CEffect* pEffect = nullptr;
        if (m_fAttackAngle > 90 && m_fAttackAngle < 270)
        {
            //여기에서 다시 각도따라 핏자국 남겨주면 됨.
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

        //슬래시 이펙트
        if (HitActor->Get_Type() == OBJ)
            pEffect = new CSlashEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Enemy_Slash", dynamic_cast<CHitBox*>(HitActor)->Get_TargetAngle());

        else if (HitActor->Get_Type() == BULLET)
            pEffect = new CSlashEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Enemy_Slash", dynamic_cast<CBullet*>(HitActor)->Get_Angle());
        
        else if (HitActor->Get_Type() == ITEM)
            pEffect = new CSlashEffect(m_tInfo.fX, m_tInfo.fY, L"Player_Enemy_Slash", dynamic_cast<CItem*>(HitActor)->Get_ThrowAngle());

        pEffect->Initialize();
        CEffectMgr::Get_Instance()->Add_Front_Effect(pEffect);

        if (g_bSlowMod)
            CScrollMgr::Get_Instance()->Shake(10, 200); //ms단위로 시간 입력.
        else
            CScrollMgr::Get_Instance()->Shake(10, 200); //ms단위로 시간 입력.
    }

    m_bDead = true; // 여기서 몬스터는 사망판정을 받음. 충돌처리니까 맨 마지막..
    Motion_Change();
}
