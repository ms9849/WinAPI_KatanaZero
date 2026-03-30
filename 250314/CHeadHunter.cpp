#include "pch.h"
#include "CHeadHunter.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CLineMgr.h"

CHeadHunter::CHeadHunter() : m_eCurMotion(IDLE), m_ePreMotion(MOTION_END), m_ePattern(NONE), m_ePrePattern(PATTERN_END),
m_bLaserShoot(false), m_pTarget(nullptr), m_bIsHitCeil(false), m_bSweepLaser(false), m_bJump(false), m_bVisible(true)
{
}

CHeadHunter::~CHeadHunter()
{
    Release();
}

void CHeadHunter::Initialize()
{
    m_fSpeed = 8.f;
    m_tInfo.fCX = 44;
    m_tInfo.fCY = 88;
    m_tSpriteInfo = m_tInfo;
    m_fTime = 0.f;
    m_fVelocity = 14.f;
    m_tCheckPoint[0] = { 1125, 558 };
    m_tCheckPoint[1] = { 235, 558 };

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Idle_12x2.bmp", L"HeadHunter_Idle");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Roll_7x2.bmp", L"HeadHunter_Roll");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Draw_Gun_8x2.bmp", L"HeadHunter_Draw_Gun");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Aim_19x2.bmp", L"HeadHunter_Aim");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Aim_Laser.bmp", L"HeadHunter_Aim_Laser");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Pre_Rush_8x2.bmp", L"HeadHunter_Pre_Rush");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Rush_1x2.bmp", L"HeadHunter_Rush");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Sweep_18x2.bmp", L"HeadHunter_Sweep");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_WallGrap_1x2.bmp", L"HeadHunter_WallGrap");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Teleport_4x2.bmp", L"HeadHunter_Teleport");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/HeadHunter/HeadHunter_Death_Jump_4x2.bmp", L"HeadHunter_Death_Jump");

    m_pFrameKey = L"HeadHunter_Idle";
    m_tFrame.iStart = 0;
    m_tFrame.iEnd = 11;
    m_tFrame.iMotion = 0;
    m_tFrame.dwTime = GetTickCount64();
    m_tFrame.dwFrameSpeed = 75.f; //추후 time 변수 추가

    m_pTarget = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
    m_fAttackDelayTime = GetTickCount64();
}

int CHeadHunter::Update()
{
    __super::Update_Rect();
    return NOEVENT;
}

void CHeadHunter::Late_Update()
{
    if (m_bDead && (m_eCurMotion == LEFT_TELEPORT || m_eCurMotion == RIGHT_TELEPORT) && isAnimationEnd())
        m_bVisible = false;

    if (m_bDead)
    {
        Death_Bound();
        __super::Update_Frame();
        __super::Update_Sprite();
        return;
    }

    Calc_Target();
    Update_Pattern();
    Dodge();
    Ground_Laser();
    Rush();
    Raser_Sweep();
    Teleport();
    __super::Update_Frame();
    __super::Update_Sprite();
    
    m_tInfo.fY += 1.0f * g_fSlowFactor;
    Motion_Change();
}

void CHeadHunter::Render(HDC hDC)
{
    if (m_bVisible == false) return;

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

    if (m_pFrameKey == L"HeadHunter_Aim" && !isAnimationEnd() && GetTickCount64() - m_fLaserDelayTime <= 500 / g_fSlowFactor)
    {
        HPEN hPen = CreatePen(PS_SOLID, (500 - (GetTickCount64() - m_fLaserDelayTime)) / 200 + 1, RGB(255, 255, 0)); // 빨간색 예시
        HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

        MoveToEx(hDC, m_tInfo.fX + 30 * cosf(m_fTargetAngle * PI / 180.f) + iScrollX, m_tInfo.fY - 30 * sinf(m_fTargetAngle * PI / 180.f) + iScrollY, NULL);
        LineTo(hDC, m_tInfo.fX + 1500 * cosf(m_fTargetAngle * PI / 180.f) + iScrollX, m_tInfo.fY - 1500 * sinf(m_fTargetAngle * PI / 180.f) + iScrollY);

        SelectObject(hDC, hOldPen);
        DeleteObject(hPen);
    }
}

void CHeadHunter::Release()
{
}

void CHeadHunter::Calc_Target()
{
    if (nullptr != m_pTarget)
    {
        float fDistX = m_pTarget->Get_Info()->fX - m_tInfo.fX;
        float fDistY = m_pTarget->Get_Info()->fY - m_tInfo.fY;

        m_fTargetDist = sqrtf(fDistX * fDistX + fDistY * fDistY);

        if (m_pTarget->Get_Info()->fY < m_tInfo.fY)
            m_fTargetAngle = acosf(fDistX / m_fTargetDist) * 180 / PI;

        else
            m_fTargetAngle = 360.f - acosf(fDistX / m_fTargetDist) * 180 / PI;
    }
}

void CHeadHunter::Update_Pattern()
{
    //sweep은 벽에 닿고 일정 좌표 이하일때 발동함.
    if (GetTickCount64() - m_fAttackDelayTime >= 350 / g_fSlowFactor && m_ePattern == NONE)
    {
        // 회피를 제외하고 아무일 없이 2초 지났다면 텔레포트로 좌표 이동
        if (GetTickCount64() - m_fAttackDelayTime >= 500 / g_fSlowFactor)
        {
            if (m_tFrame.iMotion == 0)
            {
                CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
                m_ePattern = TELEPORT;
                m_eCurMotion = RIGHT_TELEPORT;
            }
            else
            {
                CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
                m_ePattern = TELEPORT;
                m_eCurMotion = LEFT_TELEPORT;
            }
        }
        //  플레이어가 공격했을때 가깝다면, 회피
        if (m_pTarget->Get_Motion() == CPlayer::LEFT_ATTACK || m_pTarget->Get_Motion() == CPlayer::RIGHT_ATTACK)
        {
            CSoundMgr::Get_Instance()->PlaySound(L"player_roll.wav", SOUND_EFFECT, g_fVolume);
            if (m_fTargetDist <= 400 && m_pTarget->Get_Info()->fX < m_tInfo.fX)
            {
                m_ePattern = DODGE;
                m_eCurMotion = LEFT_ROLL;
                m_tFrame.iStart = 6;
            }
            else if (m_fTargetDist <= 400 && m_pTarget->Get_Info()->fX > m_tInfo.fX) 
            {
                m_ePattern = DODGE;
                m_eCurMotion = RIGHT_ROLL;
                m_tFrame.iStart = 0;
            }
        }
        // 땅 레이저 발사 패턴, 멀어야 발동
        else if (m_fTargetDist > 550 && m_ePrePattern != GROUND_LASER)
        {
            m_ePattern = GROUND_LASER;

            if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
                m_eCurMotion = LEFT_DRAW_GUN;
            else
                m_eCurMotion = RIGHT_DRAW_GUN;
        }
        // 벽 점프
        else  if (m_fTargetDist < 200 && m_ePrePattern != RUSH && GetTickCount64() - m_fAttackDelayTime >= 1500 / g_fSlowFactor)
        {
            int iNum = rand() % 2;
            m_ePattern = RUSH;
            m_eCurMotion = iNum == 1 ? LEFT_PRE_RUSH : RIGHT_PRE_RUSH;
        }

        m_ePrePattern = m_ePattern;
        Motion_Change();
    }
}

void CHeadHunter::Motion_Change()
{
    if (m_ePreMotion != m_eCurMotion)
    {
        switch (m_eCurMotion)
        {
        case IDLE:
            m_pFrameKey = L"HeadHunter_Idle";

            if (m_pTarget->Get_Info()->fX <= m_tInfo.fX) // 왼쪽 보게
            {
                m_tFrame.iStart = 11;
                m_tFrame.iEnd = 11;
                m_tFrame.iMotion = 1;
            }
            else
            {
                m_tFrame.iStart = 0;
                m_tFrame.iEnd = 11;
                m_tFrame.iMotion = 0;
            }
            m_tSpriteInfo.fCX = 44;
            m_tSpriteInfo.fCY = 88;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;
        //여기서 다른 케이스 추가.
        case LEFT_ROLL:
            m_pFrameKey = L"HeadHunter_Roll";

            m_tSpriteInfo.fCX = 118;
            m_tSpriteInfo.fCY = 96;

            m_tFrame.iStart = 6;
            m_tFrame.iEnd = 6;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;

            break;

        case RIGHT_ROLL:
            m_pFrameKey = L"HeadHunter_Roll";

            m_tSpriteInfo.fCX = 118;
            m_tSpriteInfo.fCY = 96;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 6;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case LEFT_DRAW_GUN:
            m_pFrameKey = L"HeadHunter_Draw_Gun";

            m_tSpriteInfo.fCX = 76;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 7;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;
        case RIGHT_DRAW_GUN:
            m_pFrameKey = L"HeadHunter_Draw_Gun";

            m_tSpriteInfo.fCX = 76;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case LEFT_AIM:
            m_pFrameKey = L"HeadHunter_Aim";

            m_tSpriteInfo.fCX = 78;
            m_tSpriteInfo.fCY = 104;

            m_tFrame.iStart = 9;
            m_tFrame.iEnd = -100;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 10000000;
            break;
        case RIGHT_AIM:
            m_pFrameKey = L"HeadHunter_Aim";

            m_tSpriteInfo.fCX = 78;
            m_tSpriteInfo.fCY = 104;

            m_tFrame.iStart = 9;
            m_tFrame.iEnd = 200;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 10000000;
            break;

        case LEFT_PRE_RUSH:
            m_pFrameKey = L"HeadHunter_Pre_Rush";

            m_tSpriteInfo.fCX = 114;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 7;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case RIGHT_PRE_RUSH:
            m_pFrameKey = L"HeadHunter_Pre_Rush";

            m_tSpriteInfo.fCX = 114;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 7;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case LEFT_RUSH:
            m_pFrameKey = L"HeadHunter_Rush";

            m_tSpriteInfo.fCX = 56;
            m_tSpriteInfo.fCY = 50;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;
        case RIGHT_RUSH:
            m_pFrameKey = L"HeadHunter_Rush";

            m_tSpriteInfo.fCX = 56;
            m_tSpriteInfo.fCY = 50;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case LEFT_WALLGRAP:
            m_pFrameKey = L"HeadHunter_WallGrap";

            m_tSpriteInfo.fCX = 60;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case RIGHT_WALLGRAP:
            m_pFrameKey = L"HeadHunter_WallGrap";

            m_tSpriteInfo.fCX = 60;
            m_tSpriteInfo.fCY = 86;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 30;
            break;

        case LEFT_SWEEP:
            m_pFrameKey = L"HeadHunter_Sweep";

            m_tSpriteInfo.fCX = 118;
            m_tSpriteInfo.fCY = 92;

            m_tFrame.iStart = 17;
            m_tFrame.iEnd = 17;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case RIGHT_SWEEP:
            m_pFrameKey = L"HeadHunter_Sweep";

            m_tSpriteInfo.fCX = 118;
            m_tSpriteInfo.fCY = 92;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 17;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 75;
            break;

        case LEFT_TELEPORT:
            m_pFrameKey = L"HeadHunter_Teleport";

            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 62;

            m_tFrame.iStart = 3;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case RIGHT_TELEPORT:
            m_pFrameKey = L"HeadHunter_Teleport";

            m_tSpriteInfo.fCX = 72;
            m_tSpriteInfo.fCY = 62;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case LEFT_DEATH_JUMP:
            m_pFrameKey = L"HeadHunter_Death_Jump";

            m_tSpriteInfo.fCX = 82;
            m_tSpriteInfo.fCY = 52;

            m_tFrame.iStart = 3;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;
        case RIGHT_DEATH_JUMP:
            m_pFrameKey = L"HeadHunter_Death_Jump";

            m_tSpriteInfo.fCX = 82;
            m_tSpriteInfo.fCY = 52;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 3;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case LEFT_DEATH_GROUND:
            m_pFrameKey = L"HeadHunter_Death_Ground";

            m_tSpriteInfo.fCX = 116;
            m_tSpriteInfo.fCY = 82;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 1;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;

        case RIGHT_DEATH_GROUND:
            m_pFrameKey = L"HeadHunter_Death_Ground";

            m_tSpriteInfo.fCX = 116;
            m_tSpriteInfo.fCY = 82;

            m_tFrame.iStart = 0;
            m_tFrame.iEnd = 0;
            m_tFrame.iMotion = 0;

            m_tFrame.dwTime = GetTickCount64();
            m_tFrame.dwFrameSpeed = 50;
            break;
        }


        m_ePreMotion = m_eCurMotion;
    }
}

void CHeadHunter::HitActor(CObj* HitActor, HITPOINT _HitInfo)
{
}

void CHeadHunter::HitActor(CMap* HitActor, HITPOINT _HitInfo)
{
    if (HitActor->GetObjType() == TYPE_GROUND) // 선 충돌.
    {
        if (m_eCurMotion == LEFT_DEATH_JUMP || m_eCurMotion == RIGHT_DEATH_JUMP)
        {
            m_fTime = 0.f;
            m_bJump = false;
            m_tFrame.iMotion == 1 ? m_eCurMotion = LEFT_TELEPORT : m_eCurMotion = RIGHT_TELEPORT;
            CSoundMgr::Get_Instance()->PlaySound(L"boss_teleport.wav", SOUND_EFFECT, g_fVolume);
            Motion_Change();
        }

        if (m_ePattern == SWEEP)
        {
            m_ePattern = NONE;
            m_eCurMotion = IDLE;
            m_fAttackDelayTime = GetTickCount64();

            m_bIsHitCeil = false;
            m_bSweepLaser = false;
        }
    }

    if (HitActor->GetObjType() == TYPE_WALL || HitActor->GetObjType() == TYPE_MONSTER_WALL) // 렉트 벽 충돌
    {
        if (_HitInfo.eDirection == DIRECTION::DIR_DOWN)
        {
            m_fTime = 3.9f;
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_UP)
        {
            m_tInfo.fY += fabsf(_HitInfo.fY);
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_LEFT)
        {
            m_tInfo.fX += (fabsf(_HitInfo.fX) + 4.f);
            //m_eCurMotion = LEFT_WALLGRAP;
            if (m_ePattern != DEATH)
            {
                m_ePattern = SWEEP;
                m_eCurMotion = LEFT_WALLGRAP;
            }
        }

        if (_HitInfo.eDirection == DIRECTION::DIR_RIGHT)
        {
            m_tInfo.fX -= (fabsf(_HitInfo.fX) + 4.f);

            if (m_ePattern != DEATH)
            {
                m_ePattern = SWEEP;
                m_eCurMotion = RIGHT_WALLGRAP;
            }
        }
    }
}

void CHeadHunter::HitActor(CObj* HitActor)
{
    if (HitActor->Get_Type() == OBJ && dynamic_cast<CHitBox*>(HitActor)->Get_Hit() == true)
        return;

    else if (HitActor->Get_Type() == OBJ && dynamic_cast<CHitBox*>(HitActor)->Get_Hit() == false)
            dynamic_cast<CHitBox*>(HitActor)->Set_Hit(true);

    if (m_eCurMotion != LEFT_ROLL && m_eCurMotion != RIGHT_ROLL &&
        m_bDead == false)
    {
        if (m_tFrame.iMotion == 1)
            m_eCurMotion = LEFT_DEATH_JUMP;

        else
            m_eCurMotion = RIGHT_DEATH_JUMP;

        if (m_pFrameKey != L"HeadHunter_Death_Jump" && m_pFrameKey != L"HeadHunter_Death_Ground")
            m_bJump = true;

        m_ePattern = DEATH;
        m_fAttackAngle = dynamic_cast<CHitBox*>(HitActor)->Get_TargetAngle();

        CSoundMgr::Get_Instance()->PlaySound(L"boss_hurt.wav", SOUND_EFFECT, g_fVolume);
        m_bDead = true; // 여기서 몬스터는 사망판정을 받음. 충돌처리니까 맨 마지막..
        Motion_Change();
    }
}

void CHeadHunter::Ground_Laser()
{
    if (m_ePattern == GROUND_LASER)
    {
        //총 뽑고
        if (m_pFrameKey == L"HeadHunter_Draw_Gun" && isAnimationEnd())
        {
            if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
                m_eCurMotion = LEFT_AIM;
            else
                m_eCurMotion = RIGHT_AIM;

            m_fLaserDelayTime = GetTickCount64();
            CSoundMgr::Get_Instance()->PlaySound(L"boss_aim_ground.wav", SOUND_EFFECT, g_fVolume);
        }
        else if (m_pFrameKey == L"HeadHunter_Draw_Gun" && !isAnimationEnd())
            return;
        // 조준하는 모션
        else if (m_pFrameKey == L"HeadHunter_Aim" && !isAnimationEnd() && (GetTickCount64() - m_fLaserDelayTime <= 800 / g_fSlowFactor))
        {
            if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
            {
                m_eCurMotion = LEFT_AIM;
                m_tFrame.iMotion = 1;
            }
            else
            {
                m_eCurMotion = RIGHT_AIM;
                m_tFrame.iMotion = 0;
            }

            if (m_fTargetAngle >= 90.0f && m_fTargetAngle <= 270.0f)
            {
                // 90 ~ 270 → 1 ~ 17 선형 증가
                m_tFrame.iStart = static_cast<int>(1 + ((m_fTargetAngle - 90.0f) / 180.0f) * 16.0f);
            }
            else
            {
                if (m_fTargetAngle < 90.0f)
                    m_tFrame.iStart = static_cast<int>((m_fTargetAngle / 10.0f) + 9.0f);

                else 
                    m_tFrame.iStart = static_cast<int>((m_fTargetAngle - 270.0f) / 10.0f);
            }

        }
        //레이저 히트박스 생성 후 잠깐의 대기 모션이 끝나면 발사. 레이저 히트박스는 line으로 처리하는게 나아보이긴 한다.
        else if (m_pFrameKey == L"HeadHunter_Aim" && (GetTickCount64() - m_fLaserDelayTime > 500 / g_fSlowFactor) && !m_bLaserShoot)
        {
            CSoundMgr::Get_Instance()->PlaySound(L"boss_laser_ground.wav", SOUND_EFFECT, g_fVolume);
            CLineMgr::Get_Instance()->Add_Line(new CLaser(
                { m_tInfo.fX + 30 * cosf(m_fTargetAngle * PI / 180.f), m_tInfo.fY - 30 * sinf(m_fTargetAngle * PI / 180.f) }, 
                { m_tInfo.fX + 1500 * cosf(m_fTargetAngle * PI / 180.f), m_tInfo.fY - 1500 * sinf(m_fTargetAngle * PI / 180.f) },
                TYPE_LASER));

            m_bLaserShoot = true;
        }
        else if(GetTickCount64() - m_fLaserDelayTime > 100 / g_fSlowFactor)
        {
            m_ePattern = NONE;
            m_eCurMotion = IDLE;
            m_fAttackDelayTime = GetTickCount64();
            m_bLaserShoot = false;
        }
    }
}

void CHeadHunter::Dodge()
{
    if (m_ePattern == DODGE)
    {
        if (m_eCurMotion == LEFT_ROLL)
        {
            if (m_tFrame.iStart == 0) // 프레임 재생 끝났으면,
            {
                m_eCurMotion = IDLE;
                m_ePattern = NONE; // IDLE, NONE
                //m_fAttackDelayTime = GetTickCount64() ;
            }

            m_tInfo.fX -= m_fSpeed * g_fSlowFactor;
        }
        else if (m_eCurMotion == RIGHT_ROLL)
        {
            if (m_tFrame.iStart == m_tFrame.iEnd) // 프레임 재생 끝났으면,
            {
                m_eCurMotion = IDLE;
                m_ePattern = NONE; // IDLE, NONE
                //m_fAttackDelayTime = GetTickCount64();
            }

            m_tInfo.fX += m_fSpeed * g_fSlowFactor;
        }
    
    }
}

void CHeadHunter::Rush()
{
    if (m_ePattern == RUSH)
    {
        //칼 뽑아드는 준비모션
        if (m_pFrameKey == L"HeadHunter_Pre_Rush" && isAnimationEnd())
        {
            if (m_eCurMotion == LEFT_PRE_RUSH)
                m_eCurMotion = LEFT_RUSH;
            else
                m_eCurMotion = RIGHT_RUSH;

            m_fRushAngle = m_fTargetAngle;
        }
        else if (m_pFrameKey == L"HeadHunter_Pre_Rush" && !isAnimationEnd())
            return;

        else if (m_pFrameKey == L"HeadHunter_Rush")
        {
            float angle = (m_eCurMotion == LEFT_RUSH) ? 160.f : 20.f;
            float radian = angle * PI / 180.f;

            float speed = m_fSpeed * g_fSlowFactor;

            m_tInfo.fX += (m_eCurMotion == LEFT_RUSH ? -1 : 1) * speed * 2;
            m_tInfo.fY -= speed * 1.5f * sinf(radian);
        }
    }

}

void CHeadHunter::Raser_Sweep()
{
    if (m_ePattern == SWEEP)
    {

        if (m_tInfo.fY >= 400)
        {
            if (m_eCurMotion == LEFT_WALLGRAP)
                m_eCurMotion = LEFT_SWEEP;

            else if (m_eCurMotion == RIGHT_WALLGRAP)
                m_eCurMotion = RIGHT_SWEEP;

            if (!m_bSweepLaser)
            {
                CSoundMgr::Get_Instance()->PlaySound(L"boss_aerial_sweep.wav", SOUND_EFFECT, g_fVolume);
                CLineMgr::Get_Instance()->Add_Line(new CMovingLaser(this, m_eCurMotion == LEFT_SWEEP ? CMovingLaser::LEFT : CMovingLaser::RIGHT, TYPE_LASER));
                m_bSweepLaser = true;
            }
        }

        if (m_eCurMotion == LEFT_SWEEP)
        {
            m_tInfo.fX += m_fSpeed * g_fSlowFactor;
            if (m_tInfo.fY <= 200) 
                m_bIsHitCeil = true;

            if(!m_bIsHitCeil)
                m_tInfo.fY -= m_fSpeed * g_fSlowFactor;
            else
                m_tInfo.fY += m_fSpeed * g_fSlowFactor;

        }
        else if (m_eCurMotion == RIGHT_SWEEP)
        {
            m_tInfo.fX -= m_fSpeed * g_fSlowFactor;
            if (m_tInfo.fY <= 200) 
                m_bIsHitCeil = true;

            if (!m_bIsHitCeil)
                m_tInfo.fY -= m_fSpeed * g_fSlowFactor;
            else
                m_tInfo.fY += m_fSpeed * g_fSlowFactor;
        }
    }
}

void CHeadHunter::Teleport()
{
    if (m_ePattern == TELEPORT)
    {
        if (isAnimationEnd())
        {
            int iNum = rand() % 2; 
            m_tInfo.fX = m_tCheckPoint[iNum].x;
            m_tInfo.fY = m_tCheckPoint[iNum].y;

            Calc_Target();
            if (m_fTargetDist >= 450)
            {
                m_ePattern = GROUND_LASER;
                if (m_pTarget->Get_Info()->fX < m_tInfo.fX)
                    m_eCurMotion = LEFT_DRAW_GUN;
                else
                    m_eCurMotion = RIGHT_DRAW_GUN;
            }
            else
            {
                m_ePattern = RUSH;
                int iNum = rand() % 2;
                m_ePattern = RUSH;
                m_eCurMotion = iNum == 1 ? LEFT_PRE_RUSH : RIGHT_PRE_RUSH;

            }
            m_fAttackDelayTime = GetTickCount64();
            Update_Pattern();
        }
    }
}

void CHeadHunter::Death_Bound()
{
    if (m_bDead && m_bJump)
    {
        m_fTime += 0.15f * g_fSlowFactor;

        m_tInfo.fX += m_fSpeed * cosf(m_fAttackAngle * PI / 180.f) * g_fSlowFactor;

        if (m_fSpeed * g_fSlowFactor * m_fTime - 0.5 * 3.9 * m_fTime * m_fTime * g_fSlowFactor >= -6.5f * g_fSlowFactor)
            m_tInfo.fY -= m_fSpeed * g_fSlowFactor * m_fTime - 0.5 * 3.9 * m_fTime * m_fTime * g_fSlowFactor;
        else
            m_tInfo.fY -= -6.5f * g_fSlowFactor;
    }
}
